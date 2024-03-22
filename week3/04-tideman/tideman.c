#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
int check_cycle(int winner, int loser);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(name, candidates[i]))
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // printf("\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Use pair_count as idx and update (+1) for every valid pair added
            // printf("(%d:%d v %d:%d)\n", i, preferences[i][j], j, preferences[j][i]);
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
                // printf("%d (%d, %d) - %d:%d\n", pair_count-1, pairs[pair_count-1].winner, pairs[pair_count-1].loser, preferences[pairs[pair_count-1].winner][pairs[pair_count-1].loser], preferences[pairs[pair_count-1].loser][pairs[pair_count-1].winner]);
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
                // printf("%d (%d, %d) - %d:%d\n", pair_count-1, pairs[pair_count-1].winner, pairs[pair_count-1].loser, preferences[pairs[pair_count-1].winner][pairs[pair_count-1].loser], preferences[pairs[pair_count-1].loser][pairs[pair_count-1].winner]);
            }
            // else{
            //     printf("(%d:%d v %d:%d) not added.\n", i, preferences[i][j], j, preferences[j][i]);
            // }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Sort by preference
    pair temp;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("%d (%d, %d) - %d:%d\n", i, pairs[i].winner, pairs[i].loser, preferences[pairs[i].winner][pairs[i].loser], preferences[pairs[i].loser][pairs[i].winner]);
    // }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // printf("pair #%d (%d > %d): not locked\n", i, pairs[i].winner, pairs[i].loser);
        if (!check_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            // printf("pair #%d: locked\n", i);
        }
    }
    return;
}

// Checks for any cycle formed by the node
int check_cycle(int winner, int loser)
{
    if (winner == loser)
    {
        // printf("Cycle formed\n");
        return 1;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        // printf("Checking link from %d to %d\n", loser, i);
        if (locked[loser][i] == true)
        {
            // printf("Link found from %d to %d\n", loser, i);
            if (check_cycle(winner, i))
            {
                return 1;
            }
        }
    }
    // printf("Cycle not found\n");
    return 0;
}

// Print the winner of the election
void print_winner(void)
{
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%d ", preferences[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%d ", locked[i][j]);
    //     }
    //     printf("\n");
    // }

    int winner, fails;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
            {
                winner = i;
            }
            if (locked[j][i])
            {
                break;
            }
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}
