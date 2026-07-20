#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RULES 20
#define MAX_PROD 10
#define MAX_SYMBOLS 10

// Structure to represent a grammar production rule
typedef struct {
    char head;
    char body[MAX_PROD][MAX_SYMBOLS];
    int prod_count;
} GrammarRule;

GrammarRule grammar[MAX_RULES];
int rule_count = 0;

// Arrays to store FIRST and FOLLOW sets
char first_sets[26][MAX_SYMBOLS * 2];
char follow_sets[26][MAX_SYMBOLS * 2];

// Check whether a symbol is a non-terminal
bool is_non_terminal(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z');
}

// Add a symbol to a set if not already present
bool add_to_set(char set[], char val) {
    int len = strlen(set);

    for (int i = 0; i < len; i++) {
        if (set[i] == val)
            return false;
    }

    set[len] = val;
    set[len + 1] = '\0';
    return true;
}

// Merge one set into another
bool merge_sets(char set1[], char set2[], bool ignore_epsilon) {
    bool changed = false;

    for (int i = 0; set2[i] != '\0'; i++) {
        if (ignore_epsilon && set2[i] == 'e')
            continue;

        if (add_to_set(set1, set2[i]))
            changed = true;
    }

    return changed;
}

// Check if a set contains epsilon
bool has_epsilon(char set[]) {
    for (int i = 0; set[i] != '\0'; i++) {
        if (set[i] == 'e')
            return true;
    }

    return false;
}

// Compute FIRST sets
void compute_first() {

    bool changed = true;

    while (changed) {

        changed = false;

        for (int i = 0; i < rule_count; i++) {

            char head = grammar[i].head;
            int head_idx = head - 'A';

            for (int j = 0; j < grammar[i].prod_count; j++) {

                char *prod = grammar[i].body[j];

                if (prod[0] == 'e') {

                    if (add_to_set(first_sets[head_idx], 'e'))
                        changed = true;

                } else {

                    bool all_epsilon = true;

                    for (int k = 0; prod[k] != '\0'; k++) {

                        char sym = prod[k];

                        if (!is_non_terminal(sym)) {

                            if (add_to_set(first_sets[head_idx], sym))
                                changed = true;

                            all_epsilon = false;
                            break;

                        } else {

                            int sym_idx = sym - 'A';

                            if (merge_sets(first_sets[head_idx], first_sets[sym_idx], true))
                                changed = true;

                            if (!has_epsilon(first_sets[sym_idx])) {
                                all_epsilon = false;
                                break;
                            }
                        }
                    }

                    if (all_epsilon) {
                        if (add_to_set(first_sets[head_idx], 'e'))
                            changed = true;
                    }
                }
            }
        }
    }
}

// Compute FOLLOW sets
void compute_follow(char start_symbol) {

    add_to_set(follow_sets[start_symbol - 'A'], '$');

    bool changed = true;

    while (changed) {

        changed = false;

        for (int i = 0; i < rule_count; i++) {

            char head = grammar[i].head;
            int head_idx = head - 'A';

            for (int j = 0; j < grammar[i].prod_count; j++) {

                char *prod = grammar[i].body[j];
                int len = strlen(prod);

                for (int k = 0; k < len; k++) {

                    char current = prod[k];

                    if (is_non_terminal(current)) {

                        int cur_idx = current - 'A';
                        bool end = true;

                        for (int n = k + 1; n < len && end; n++) {

                            char next = prod[n];

                            if (!is_non_terminal(next)) {

                                if (add_to_set(follow_sets[cur_idx], next))
                                    changed = true;

                                end = false;

                            } else {

                                int next_idx = next - 'A';

                                if (merge_sets(follow_sets[cur_idx],
                                               first_sets[next_idx], true))
                                    changed = true;

                                if (!has_epsilon(first_sets[next_idx]))
                                    end = false;
                            }
                        }

                        if (end) {

                            if (merge_sets(follow_sets[cur_idx],
                                           follow_sets[head_idx], false))
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

int main() {

    // Initialize FIRST and FOLLOW sets
    for (int i = 0; i < 26; i++) {
        first_sets[i][0] = '\0';
        follow_sets[i][0] = '\0';
    }

    // Grammar:
    // S -> C B
    grammar[rule_count].head = 'S';
    strcpy(grammar[rule_count].body[0], "CB");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    // C -> cC | d
    grammar[rule_count].head = 'C';
    strcpy(grammar[rule_count].body[0], "cC");
    strcpy(grammar[rule_count].body[1], "d");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    // B -> bC
    grammar[rule_count].head = 'B';
    strcpy(grammar[rule_count].body[0], "bC");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    char start_symbol = 'S';

    compute_first();
    compute_follow(start_symbol);

    printf("========== FIRST SETS ==========\n");

    for (int i = 0; i < rule_count; i++) {

        char head = grammar[i].head;

        printf("FIRST(%c) = { ", head);

        char *s = first_sets[head - 'A'];

        for (int j = 0; s[j] != '\0'; j++) {
            printf("%c", s[j]);

            if (s[j + 1] != '\0')
                printf(", ");
        }

        printf(" }\n");
    }

    printf("\n========== FOLLOW SETS ==========\n");

    for (int i = 0; i < rule_count; i++) {

        char head = grammar[i].head;

        printf("FOLLOW(%c) = { ", head);

        char *s = follow_sets[head - 'A'];

        for (int j = 0; s[j] != '\0'; j++) {

            printf("%c", s[j]);

            if (s[j + 1] != '\0')
                printf(", ");
        }

        printf(" }\n");
    }

    return 0;
}