#include <bits/stdc++.h>
using namespace std;

/* GLOBAL VARIABLES USED THROUGHOUT THE CODE */
int NUMBER_OBJECTS;     // populated automatically by processData
int NUMBER_CONSTRAINTS; // populated automatically by processData
int OPTIMUM;            // populated automatically by processData
int *CAPACITIES;        // populated automatically by processData
int **CONSTRAINTS;      // populated automatically by processData
int *VALUES;            // populated automatically by processData

const int POPULATION = 10;        // size of the population
const int LOCAL_IMPROVEMENT = 10; // number of local improvements
const int GENERATIONS = 100;      // number of generations to run the algorithm

int *WEIGHTED_SUMS; // weighted sum for the corresponding constraint

struct Item
{
    int value;
    double efficiency;
};
vector<Item> ITEMS; // efficiencies of the objects

void processData(char *filename)
{
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Data File Not Found in Current Directory.");
        exit(1);
    }
    char *line = new char[1000];
    fgets(line, 1000, file);
    char *tok = strtok(line, " \t");
    NUMBER_CONSTRAINTS = atoi(tok);
    tok = strtok(NULL, " \t");
    NUMBER_OBJECTS = atoi(tok);
    int i = 0;

    /* VALUES (objective function) */
    VALUES = new int[NUMBER_OBJECTS];
    fgets(line, 1000, file);
    tok = strtok(line, " \t");
    while (true)
    {
        while (tok != NULL && i < NUMBER_OBJECTS)
        {
            int vv = atoi(tok);
            VALUES[i] = vv;
            i++;
            tok = strtok(NULL, " \t");
            if (tok == NULL)
            {
                break;
            }

            if (strcmp(tok, " ") == 0 || strcmp(tok, "") == 0)
            {
                continue;
            }

            if (strcmp(tok, "\n") == 0)
            {
                break;
            }
        }

        if (i < NUMBER_OBJECTS)
        {
            fgets(line, 1000, file);
            tok = strtok(line, " \t");
            int vv = atoi(tok);
            VALUES[i] = vv;
            tok = strtok(NULL, " \t");
            i++;
        }
        else
        {
            break;
        }
    }

    /* CAPACITIES */
    CAPACITIES = new int[NUMBER_CONSTRAINTS];
    i = 0;
    fgets(line, 1000, file);
    tok = strtok(line, " \t");
    while (true)
    {
        while (tok != NULL && i < NUMBER_CONSTRAINTS)
        {
            int vv = atoi(tok);
            CAPACITIES[i] = vv;
            i++;
            tok = strtok(NULL, " \t");
            if (tok == NULL)
            {
                break;
            }

            if (strcmp(tok, " ") == 0 || strcmp(tok, "") == 0)
            {
                continue;
            }

            if (strcmp(tok, "\n") == 0)
            {
                break;
            }
        }

        if (i < NUMBER_CONSTRAINTS)
        {
            fgets(line, 1000, file);
            tok = strtok(line, " \t");
            int vv = atoi(tok);
            CAPACITIES[i] = vv;
            i++;
            tok = strtok(NULL, " \t");
        }
        else
        {
            break;
        }
    }

    /* CONSTRAINTS */
    CONSTRAINTS = new int *[NUMBER_CONSTRAINTS];
    for (int n = 0; n < NUMBER_CONSTRAINTS; n++)
    {
        i = 0;
        fgets(line, 1000, file);
        CONSTRAINTS[n] = new int[NUMBER_OBJECTS];
        tok = strtok(line, " \t");
        int vv = atoi(tok);
        CONSTRAINTS[n][i] = vv;
        i++;
        while (true)
        {
            tok = strtok(NULL, " \t");
            while (tok != NULL && i < NUMBER_OBJECTS)
            {
                vv = atoi(tok);
                CONSTRAINTS[n][i] = vv;
                tok = strtok(NULL, " \t");
                i++;
                if (tok == NULL)
                {
                    break;
                }

                if (strcmp(tok, " ") == 0 || strcmp(tok, "") == 0)
                {
                    continue;
                }

                if (strcmp(tok, "\n") == 0)
                {
                    break;
                }
            }

            if (i < NUMBER_OBJECTS)
            {
                fgets(line, 1000, file);
                tok = strtok(line, " \t");
                int vv = atoi(tok);
                CONSTRAINTS[n][i] = vv;
                i++;
            }
            else
            {
                break;
            }
        }
    }

    fgets(line, 1000, file);
    tok = strtok(line, " \t");
    while (strcmp(tok, "") == 0 || strcmp(tok, " ") == 0 || strcmp(tok, "\n") == 0)
    {
        fgets(line, 1000, file);
        tok = strtok(line, " \t");
    }

    OPTIMUM = atoi(tok);

    delete[] (line);

    // for (int i = 0; i < NUMBER_OBJECTS; ++i)
    //     printf("%d: %d\n", i, VALUES[i]);
    // cout << endl;
    // for (int i = 0; i < NUMBER_CONSTRAINTS; ++i)
    //     printf("%d: %d\n", i, CAPACITIES[i]);
    // cout << endl;
    // for (int i = 0; i < NUMBER_CONSTRAINTS; ++i)
    // {
    //     printf("%d:\n", i);
    //     for (int j = 0; j < NUMBER_OBJECTS; ++j)
    //         printf("%d: %d, ", j, CONSTRAINTS[i][j]);
    //     cout << endl;
    // }
}

/* Evaluating the weighted sums */
void getWeightedSums()
{
    int *ws = new int[NUMBER_CONSTRAINTS];
    for (int i = 0; i < NUMBER_CONSTRAINTS; i++)
    {
        long sum = 0;
        for (int j = 0; j < NUMBER_OBJECTS; j++)
        {
            sum += CONSTRAINTS[i][j];
        }
        ws[i] = sum;
    }
    WEIGHTED_SUMS = ws;
}

/* Evaluating the utility ratio values */
double *getUtilityRatios()
{
    double *ratios = new double[NUMBER_CONSTRAINTS];
    getWeightedSums();
    for (int i = 0; i < NUMBER_CONSTRAINTS; i++)
    {
        ratios[i] = (WEIGHTED_SUMS[i] - CAPACITIES[i]) / (double)(WEIGHTED_SUMS[i]);
    }
    return ratios;
}

/* Evaluating efficiencies of the items */
void evaluateEfficiencies()
{
    double *ratios = getUtilityRatios();
    for (int i = 0; i < NUMBER_OBJECTS; i++)
    {
        double new_weight = 0;
        for (int j = 0; j < NUMBER_CONSTRAINTS; j++)
        {
            new_weight += (ratios[j] * CONSTRAINTS[j][i]);
        }
        ITEMS.push_back({VALUES[i], VALUES[i] / new_weight});
    }
}

/* Sorting the solutions by their efficiencies */
bool compareItem(Item &i1, Item &i2)
{
    return (i1.efficiency < i2.efficiency);
}
void sortByEfficiencies()
{
    evaluateEfficiencies();
    sort(ITEMS.begin(), ITEMS.end(), compareItem);
}

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        printf("Usage: ./a.out <datafile> <weing/orlib>\n");
        printf("Examples:\n");
        printf("./a.out data.DAT weing\n\n");
        exit(0);
    }
    if (strcmp(argv[2], "weing") == 0)
    {
        printf("Data file format: weing\n");
        printf("Data file name: %s\n", argv[1]);
        processData(argv[1]);
    }
    else
    {
        printf("Unknown file format. Exiting...\n");
        exit(1);
    }

    // efficiency calculation of the values
    sortByEfficiencies();

    for (int i = 0; i < NUMBER_OBJECTS; i++)
    {
        printf("%d: %d\t%f\n", i, ITEMS[i].value, ITEMS[i].efficiency);
    }
}
