#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TxLib.h"
#include "Stack.h"

//=============================================================================

struct node_t
{
    char *data;
    node_t *left;
    node_t *right;
};

//=============================================================================

void Menu (node_t *node, FILE *fin,  FILE *fout);

//=============================================================================

void Play (node_t *node, FILE *fout);

void Definition (node_t *node);

bool Find_Same (node_t *node, const char *name, stack_t *way);

void Difference (node_t *node);

void Print_Definition_Of_Name (node_t *node, const int *res, const int len);

void Print_Name1_And_Name2 (const char *name1, const char *name2);

//=============================================================================

node_t *Create_Node (char *data);

node_t *Make_Tree (FILE *fin);

void *Add_Node (node_t *node);

//-----------------------------------------------------------------------------

void PNG_Dump (node_t *node);

void Print_PNG (node_t *node, FILE *fout);

//-----------------------------------------------------------------------------

void Make_Fout  (node_t *node, FILE *fout);

void Print_Fout (node_t *node, FILE *fout);

//=============================================================================

int main ()
{
    char *name_of_fin = "input.txt";

    FILE *fin   = fopen (name_of_fin,  "r");
    FILE *fout  = fopen ("output.txt", "r+");

    node_t *node = Make_Tree (fin);

    Menu (node, fin, fout);

    fclose (fin);
    fclose (fout);

    return 0;
}

//=============================================================================

void Menu (node_t *node, FILE *fin,  FILE *fout)
{
    while (true)
    {
        PR_B(Choose mode:\n, Blue);

        printf ("Type [");       // Type [0] to exit
        PR_B(0, White)
        printf ("] to ");
        PR_B(exit\n, Green)

        printf ("Type [");       // Type [1] to play
        PR_B(1, White)
        printf("] to ");
        PR_B(play\n, Green)

        printf ("Type [");       // Type [2] to definition
        PR_B(2, White)
        printf ("] to ");
        PR_B(definition\n, Green);

        printf ("Type [");       // Type [3] to difference
        PR_B(3, White)
        printf ("] to ");
        PR_B(difference\n, Green)

        char helper[50] = {};
        gets (helper);

        switch (helper[0])
        {
            case '0':
            {
                return ;
            }

            case '1':
            {
                txCreateWindow (1600, 1000);

                while (!GetAsyncKeyState (VK_SHIFT))
                {
                    Play (node, fout);
                    fseek (fout, 0, SEEK_SET);
                    Make_Fout (node, fout);
                    fseek (fout, 0, SEEK_SET);
                    node = Make_Tree (fout);
                }

                break;
            }

            case '2':
            {
                Definition (node);
                break;
            }

            case '3':
            {
                Difference (node);
                break;
            }

            default:
                CH(Red)
                printf ("\nWrong, try again!\n");
                CH_S
        }
    }
}

//=============================================================================

void Play (node_t *node, FILE *fout)
{
    double x0 = 760;
    double y0 = 300;

    txSelectFont ("Comic Sans MS", 56);
    txSetFillColor (TX_BLACK);

    int ok1 = 0;
    while (!GetAsyncKeyState (VK_SHIFT))
    {
        if (ok1 == 0)
        {
            txClear ();
            if (node -> left != nullptr || node -> right != nullptr)
            {
                txTextOut (x0 - strlen (node -> data) * 8 - 10, y0, node -> data);
                txTextOut (x0 + strlen (node -> data) * 13 - 10, y0, "?");
                txTextOut (x0 - 150, y0 + 100, "NO");
                txTextOut (x0 + 100, y0 + 100, "YES");
                ok1 = 1;
            }
            else
            {
                txTextOut (x0 - strlen (node -> data) * 8 - 24, y0 - 50, "ANSWER:");
                txSetColor (TX_ORANGE);
                txTextOut (x0 - strlen (node -> data) * 8 - 6, y0, node -> data);
                txSetColor (TX_WHITE);
                txTextOut (x0 - strlen (node -> data) * 8, y0 + 70, "True?");
                txTextOut (x0 - 180, y0 + 150, "NO");
                txTextOut (x0 + 100, y0 + 150, "YES");
                ok1 = 1;

                int ok2 = 0;
                while (ok2 == 0)
                {
                    if (GetAsyncKeyState (VK_LEFT))
                    {
                        Add_Node (node);

                        return ;
                    }
                    else if (GetAsyncKeyState (VK_RIGHT))
                    {
                        txClear ();
                        txSetColor (TX_MAGENTA);
                        txSelectFont ("Arial", 100, 40, 700, "italic");
                        txTextOut (x0 - 200, y0, "I KNEW IT!");
                        ok2 = 1;
                        txSleep (300);
                    }
                }
            }
        }

        if (GetAsyncKeyState (VK_RIGHT))
        {
            if (node -> right)
            {
                txSetColor (TX_YELLOW);
                txTextOut (x0 + 100, y0 + 100, "YES");
                txSetColor (TX_WHITE);
                node = node -> right;
                ok1 = 0;
                txSleep (300);
            }

        }
        if (GetAsyncKeyState (VK_LEFT))
        {
            if (node -> left)
            {
                txSetColor (TX_YELLOW);
                txTextOut (x0 - 150, y0 + 100, "NO");
                txSetColor (TX_WHITE);
                node = node -> left;
                ok1 = 0;
                txSleep (300);
            }
        }
    }
}

//=============================================================================

void Definition (node_t *node)
{
    char name[20] = {};
    PR_B(Type name definition of what you want:\n, Green)

    gets (name);

    stack_t *way = nullptr;
    STACK_CONSTRUCTOR(way)

    if (!Find_Same (node, name, way))
    {
        CH(Red)
        printf ("ERROR!!! NO ELEM");
        CH(Yellow)
        printf (" %s\n\n", name);
        CH_S

        return ;
    }

    int *res = (int *)(way -> arr);

    CH(Yellow)
    printf ("%s", name);
    CH_S
    printf (":");

    Print_Definition_Of_Name (node, res, way->size - 1);
}

//-----------------------------------------------------------------------------

bool Find_Same (node_t *node, const char *name, stack_t *way)
{
    int helper = C_poison;

    if (node -> left)
    {
        Stack_Push (way, -1);

        if (!Find_Same (node -> left, name, way))
        {
            Stack_Pop (way, &helper);
        }
        else
        {
            return true;
        }
    }

    if (node -> right)
    {
        Stack_Push (way, 1);

        if (!Find_Same (node -> right, name, way))
        {
            Stack_Pop (way, &helper);
        }

        else
        {
            return true;
        }
    }

    if (strcmp ((node -> data), name) == 0)
    {
        return true;
    }

    return false;
}

//=============================================================================

void Difference (node_t *node)
{
    char name1[20] = {};
    char name2[20] = {};

    stack_t *way1 = nullptr;
    STACK_CONSTRUCTOR(way1)
    stack_t *way2 = nullptr;
    STACK_CONSTRUCTOR(way2)

    PR_B(Type first name:\n, Green);
    gets(name1);

    while (!Find_Same (node, name1, way1))
    {
        PR_B(ERROR!!! NO ELEM, Red)
        CH(Yellow)
        printf (" %s\n\n", name1);
        PR_B(Try again:\n, Green)
        PR_B(Type first name:\n, Green);

        gets(name1);
    }

    PR_B(Type second name:\n, Green);
    gets(name2);

    while (!Find_Same (node, name2, way2))
    {
        PR_B(ERROR!!! NO ELEM, Red)
        CH(Yellow)
        printf (" %s\n\n", name2);
        PR_B(Try again:\n, Green)
        PR_B(Type second name:\n, Green);

        gets(name2);
    }

    int *res1 = (int *)(way1 -> arr);
    int *res2 = (int *)(way2 -> arr);

    int len1 = way1->size - 1;
    int len2 = way2->size - 1;
    int pos = 0;

    if (res1[0] != res2[0])
    {
        Print_Name1_And_Name2 (name1, name2);
        printf ("have no same:\n");

        CH(Yellow)
        printf ("%s", name1);
        CH_S
        printf (":");

        Print_Definition_Of_Name (node, res1, len1);

        CH(Yellow)
        printf ("%s", name2);
        CH_S
        printf (":");

        Print_Definition_Of_Name (node, res2, len2);
    }
    else if (strcmp (name1, name2) == 0)
    {
        Print_Name1_And_Name2 (name1, name2);
        printf (" are same and: ");
        Print_Definition_Of_Name (node, res1, len1);
    }
    else
    {
        Print_Name1_And_Name2 (name1, name2);
        printf (" are:");

        while (res1[pos] == res2[pos])
        {
            if (res1[pos] == -1)
            {
                PR_B(NE, Red)
                printf (" %s, ", node->data);
                node = node->left;
            }
            else
            {
                printf (" %s, ", node->data);
                node = node->right;
            }

            pos++;
        }

        printf ("\n but %s:", name1);
        Print_Definition_Of_Name (node, res1 + pos, len1);

        printf ("%s:", name2);
        Print_Definition_Of_Name (node, res2 + pos, len2);
    }
}

//-----------------------------------------------------------------------------

void Print_Definition_Of_Name (node_t *node, const int *res, const int len)
{
    for (int i = 0; i < len; i++)
    {
        if (res[i] == -1)
        {
            PR_B(NE, Red)
            printf (" %c%s, ", tolower ((node->data)[0]), node->data + 1);
            node = node->left;
        }
        else if (res[i] ==  1)
        {
            printf (" %c%s, ", tolower ((node->data)[0]), node->data + 1);
            node = node->right;
        }
    }

    if (res[len] == -1)
    {
        PR_B(NE, Red)
    }

    printf (" %c%s!\n\n", tolower ((node->data)[0]), node->data + 1);
}


//-----------------------------------------------------------------------------

void Print_Name1_And_Name2 (const char *name1, const char *name2)
{
    CH(Yellow)
    printf ("%s ", name1);
    CH_S
    printf ("and ");
    CH(Yellow)
    printf ("%s ", name2);
    CH_S
}

//=============================================================================

node_t* Create_Node (char *data)
{
    node_t *node = (node_t *) calloc (1, sizeof (node_t) + 2);
    (node -> data) = (char *) calloc (25, sizeof (char));

    strcpy (node -> data, data);
    node -> left  = nullptr;
    node -> right = nullptr;

    return node;
}

//=============================================================================

node_t *Make_Tree (FILE *fin)
{
    char *line = (char *) calloc (20, sizeof (char));

    if (fscanf (fin, " %[{]s", line) == EOF)
        return nullptr;

    fscanf (fin, "%[']s",  line);
    fscanf (fin, "%[^']s", line);

    node_t *node = Create_Node (line);

    if (fscanf (fin, "%[']s", line) == EOF)
        return nullptr;

    if (fscanf (fin, " %[^{ ]s", line) == EOF)
        return nullptr;

    if (strcmp (line, "nill") == 0)                // no left
    {
        fscanf (fin, " %[}]s ", line);             // no left,  no right

        if (strcmp (line, "nill") == 0)
        {
            return node;
        }
        else
        {
            node -> right = Make_Tree (fin);       // no left,     right
            fscanf (fin, "%[}]s", line);
            fgetc (fin);

            return node;
        }
    }
    else
    {
        node -> left = Make_Tree (fin);
        fscanf (fin, "%[}]s", line);
        fgetc (fin);

        fscanf (fin, " %s ", line);
        if (strcmp (line, "nill") == 0)            // left,  no right
            return node;

        else                                       // left,     right
        {
            node -> right = Make_Tree (fin);
            fscanf (fin, "%[}]s", line);
            fgetc (fin);

            return node;
        }
    }

    printf ("WTF??");

    return node;
}

//=============================================================================

void *Add_Node (node_t *node)
{
    char *answer = (char *) txInputBox ("Inter right answer", "DK");
    node -> right = Create_Node (answer);
    node -> left  = Create_Node (node -> data);

    char difference[50] = "Inter difference between ";

    strcat (difference, answer);
    strcat (difference, " and ");
    strcat (difference, node -> data);

    (node -> data) = (char *) calloc (50, sizeof (char));
    node -> data = (char *) txInputBox (difference, "DK");
}

//=============================================================================

void PNG_Dump (node_t *node)
{
    FILE *fout = fopen ("DOT", "w");

    fprintf (fout, "digraph\n");
    fprintf (fout, "{\n");

    Print_PNG (node, fout);

    fprintf (fout, "}");

    fclose (fout);
}

//-----------------------------------------------------------------------------

void Print_PNG (node_t *node, FILE *fout)
{
    if (node -> left)
    {
        fprintf (fout, "\"%s\"->\"%s\";\n", node -> data, (node -> left) -> data);
        Print_PNG (node -> left, fout);
    }

    if (node -> right)
    {
        fprintf (fout, "\"%s\"->\"%s\";\n", node -> data, (node -> right) -> data);
        Print_PNG (node -> right, fout);
    }
}

//=============================================================================

void Make_Fout (node_t *node, FILE *fout)
{
    fseek (fout, 0, SEEK_SET);
    Print_Fout (node, fout);
}

//-----------------------------------------------------------------------------

void Print_Fout (node_t *node, FILE *fout)
{
    fprintf (fout, " {'%s'", node -> data);

    if (node -> left)
    {
        Print_Fout (node -> left, fout);
    }
    else
    {
        fprintf (fout, " nill");
    }

    if (node -> right)
    {
        Print_Fout (node -> right, fout);
    }
    else
    {
        fprintf (fout, " nill");
    }

    fprintf (fout, "}");
}
