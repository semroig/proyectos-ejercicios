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
}
pair;

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
        //I compare the name giveng with de strings on the candidates array
        int valid = strcmp(candidates[i], name);
        if (valid == 0)
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
    for (int i = 0; i < candidate_count; i++)
    {
        int pref = ranks[i];
        for (int j = i + 1; j < candidate_count; j++)
        {
            int notpref = ranks[j];
            preferences[pref][notpref]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int ida = preferences[i][j];
            int vuelta = preferences[j][i];
            //I compare de number of voters, one over the other candidate
            if (ida > vuelta)
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                k++;
            }
        }
    }
    pair_count = k;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //I use the bubble sorting technic
    int bubble = 1;
    int sorting = pair_count;
    while (bubble != 0)
    {
        bubble = 0;
        for (int i = 0; i < sorting; i++)
        {
            if (i == sorting - 1)
            {
                sorting = sorting - 1;
            }
            else
            {
                //I store the information in this four new variables
                int wcan = pairs[i].winner;
                int lcan = pairs[i].loser;
                int left = preferences[wcan][lcan];
                int wcan2 = pairs[i + 1].winner;
                int lcan2 = pairs[i + 1].loser;
                int right = preferences[wcan2][lcan2];
                if (left < right)
                {
                    pairs[i].winner = wcan2;
                    pairs[i].loser = lcan2;
                    pairs[i + 1].winner = wcan;
                    pairs[i + 1].loser = lcan;
                    bubble++;
                }
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int base = pairs[i].winner;
        int end = pairs[i].loser;
        bool check = false;
        for (int j = 0; j < candidate_count; j++)
        {
            //Checking that all are false
            check = locked[end][j];
            if (check == true)
            {
                break;
            }
        }
        if (check == false)
        {
            locked[base][end] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //I look for a column that have all false
    string winner = "nothing";
    for (int i = 0; i < candidate_count; i++)
    {
        int check = false;
        for (int j = 0; j < candidate_count; j++)
        {
            check = locked[j][i];
            if (check == true)
            {
                break;
            }
        }
        if (check == false)
        {
            winner = candidates[i];
            break;
        }
    }
    printf("%s\n", winner);
    return;
}