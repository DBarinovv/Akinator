#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TxLib.h"

//=============================================================================

struct node_t
{
    char *data;
    node_t *left;
    node_t *right;
};

//=============================================================================

node_t *Create_Node (char *data);

node_t *Make_Tree (FILE *fin);

void *Add_Node (node_t *node);

//-----------------------------------------------------------------------------

void B_Dump (node_t *node, const double x0, const double y0, const double x1, const double y1);

void PNG_Dump (node_t *node);

void FS (node_t *node, FILE *fout);

//=============================================================================

int main ()
{
    char *name_of_fin = "input.txt";

    FILE *fin  = fopen (name_of_fin,  "r");
    FILE *fout = fopen ("output.txt", "w");

    node_t *node = Make_Tree (fin);

    txCreateWindow (1600, 1000);

    double x0 = 760;
    double y0 = 300;
    double x1 = 830;
    double y1 = 400;

//    txRectangle (x0, y0, x1, y1);
    txSelectFont ("Comic Sans MS", 56);
    txSetFillColor (TX_BLACK);

    int ok = 0;
    while (!GetAsyncKeyState (VK_ESCAPE))
    {
        if (ok == 0)
        {
            txClear();
            if (node -> left != nullptr || node -> right != nullptr)
            {
                txTextOut (x0 - strlen (node -> data) * 8, y0, node -> data);
                txTextOut (x0 - 150, y0 + 100, "NO");
                txTextOut (x0 + 100, y0 + 100, "YES");
                ok = 1;
            }
            else
            {
                txTextOut (x0 - strlen (node -> data) * 8 - 44, y0 - 50, "ANSWER:");
                txTextOut (x0 - strlen (node -> data) * 8, y0, node -> data);
                ok = 1;
            }
        }

        if (GetAsyncKeyState (VK_RIGHT))
        {
            if (node -> right)
            {
                node = node -> right;
                ok = 0;
                txSleep (300);
            }

        }
        if (GetAsyncKeyState (VK_LEFT))
        {
            if (node -> left)
            {
                node = node -> left;
                ok = 0;
                txSleep (300);
            }
        }
    }
//    txInputBox ("AAA", "Katya");
//    PNG_Dump (node);
//    B_Dump (node, x0, y0, x1, y1);

    fclose (fin);
    fclose (fout);

    return 0;
}

//=============================================================================

node_t* Create_Node (char *data)
{
    node_t *node = (node_t *) calloc (1, sizeof (node_t));
    (node -> data) = (char *) calloc (20, sizeof (1));

    strcpy (node -> data, data);
    node -> left  = nullptr;
    node -> right = nullptr;

    return node;
}

//=============================================================================

node_t *Make_Tree (FILE *fin)
{
    char *line = (char *) calloc (20, sizeof (char));

    if (fscanf (fin, "%[{]s", line) == EOF)
        return nullptr;

    fscanf (fin, "%[']s",  line);
    fscanf (fin, "%[^']s", line);

    node_t *node = Create_Node (line);

    if (fscanf (fin, "%[']s", line) == EOF)
        return nullptr;


    if (fscanf (fin, " %[^{ ]s", line) == EOF)
        return nullptr;

//    printf ("Line = %s\n", line);

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

    strcpy (difference + strlen (difference), answer);
    strcpy (difference + strlen (difference), " and ");
    strcpy (difference + strlen (difference), node -> data);

    node -> data = (char *) txInputBox (difference, "DK");
}
//=============================================================================

void B_Dump (node_t *node, const double x0, const double y0, const double x1, const double y1)
{
        txDrawText (x0, y0, x1, y1, node -> data);

        if (node -> left)
        {
            txLine (x0, y1, (x0 + x1) / 4 + 35, y0 + 25);
            B_Dump (node -> left, (x0 + x1) / 4 - 35, y0 + 25, (x0 + x1) / 4 + 35, y1 + 25);
        }

        if (node -> right)
        {
            txLine (x1, y1, 1000 - ((x0 + x1) / 2) / 2 - 35, y0 + 25);
            B_Dump (node -> right, (1000 - (x0 + x1) / 2) / 2 - 35 , y0 + 25, (1000 - (x0 + x1) / 2) / 2 + 35, y1 + 25);
        }
}

//=============================================================================

void PNG_Dump (node_t *node)
{
    FILE *fout = fopen ("DOT", "w");

    fprintf (fout, "digraph\n");
    fprintf (fout, "{\n");

    FS (node, fout);

    fprintf (fout, "}");

    fclose (fout);
}

//=============================================================================

void FS (node_t *node, FILE *fout)
{
    if (node -> left)
    {
        fprintf (fout, "\"%s\"->\"%s\";\n", node -> data, (node -> left) -> data);
        FS (node -> left, fout);
    }

    if (node -> right)
    {
        fprintf (fout, "\"%s\"->\"%s\";\n", node -> data, (node -> right) -> data);
        FS (node -> right, fout);
    }
}
