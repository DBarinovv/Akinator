typedef int elem_t;

struct stack_t
{
    int saver_left;

    const char *name;

    long int hash;

    int size;
    int capacity;

    elem_t arr[1];
};

//=============================================================================

enum errors {Size_less_0 = 1000,
             Size_more_Max_Size,
             Invalid_array,
             Try_to_change_array_after_destruct,
             Try_to_change_size_after_destruct,
             Push_out_of_range,
             Pop_out_of_range,
             Saver_left_is_broken,
             Saver_right_is_broken,
             Control_sum_changed
             };

//-----------------------------------------------------------------------------

// d - dark
enum colors {D_blue = 1, D_green, D_cyan, D_red, D_purple, D_yellow, D_white, Grey,
               Blue,       Green,   Cyan,   Red,   Purple,   Yellow,   White};

//=============================================================================

const elem_t C_poison = -8;                                                                                                                                                                                                                      const elem_t C_secret = 179;
const int C_start_capacity = 40;

const int      C_error_size     = -10;
const stack_t* C_error_null_ptr = (stack_t *)5;

const int C_saver_left     = 0xDED;
const elem_t C_saver_right = 0xDED;

//=============================================================================

#define PR_B(elem, color)     \
{                             \
    txSetConsoleAttr (color); \
    printf (#elem);           \
    txSetConsoleAttr (7);     \
}

//-----------------------------------------------------------------------------

// CH - change color
#define CH(color) \
    txSetConsoleAttr (color);

//-----------------------------------------------------------------------------

// CH_S - change color to standard
#define CH_S \
    txSetConsoleAttr (7);

//-----------------------------------------------------------------------------

// output elem in color in brackets
#define BRACKETS(bracket_1, bracket_2, elem, color) \
{                                                   \
    printf (#bracket_1);                            \
    CH(color)                                       \
    printf (#elem);                                 \
    CH_S                                            \
    printf (#bracket_2);                            \
}

//-----------------------------------------------------------------------------

#define DUMP(error, line) \
    Dump (this_, line, error, #error);

//-----------------------------------------------------------------------------

#define STACK_CONSTRUCTOR(elem)                                                                                       \
{                                                                                                                     \
    elem = (stack_t *) ((char *)calloc (sizeof (stack_t) + (C_start_capacity) * sizeof (elem_t), sizeof(char)));      \
    Stack_Constructor (elem);                                                                                         \
                                                                                                                      \
    (*elem).name = #elem;                                                                                             \
                                                                                                                      \
}

//=============================================================================

bool Stack_Constructor (stack_t* this_);

bool Stack_Destruct    (stack_t* this_);

bool Stack_Realloc     (stack_t* this_);

//-----------------------------------------------------------------------------

bool Stack_Push  (stack_t* this_, elem_t elem);

bool Stack_Pop   (stack_t* this_, elem_t *elem);

bool Stack_Empty (const stack_t* const this_);

elem_t Stack_Top (const stack_t* const this_);

int Stack_Size   (const stack_t* const this_);

bool Stack_Full  (const stack_t* const this_);

//-----------------------------------------------------------------------------

bool Dump (const stack_t* const this_, const int line, const int error, const char * error_name);

//=============================================================================

bool Stack_Constructor (stack_t* this_)
{
    (this_ -> hash) = 0;
    (this_ -> size) = 0;

    (this_ -> capacity)   = C_start_capacity;
    (this_ -> saver_left) = C_saver_left;

    for (int i = 0; i <= (this_ -> capacity) - 1; i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    (this_ -> arr) [(this_ -> capacity)] = C_saver_right;

    return true;
}
//=============================================================================

bool Stack_Destruct (stack_t* this_)
{
    for (int i = 0; i <= (this_ -> capacity); i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    this_ = (stack_t *)C_error_null_ptr;
    free (this_);

    (this_ -> size) = C_error_size;

    return true;
}

//=============================================================================

bool Stack_Realloc (stack_t* this_)
{
    (this_ -> capacity) = (this_ -> capacity) * 2;

    this_ = (stack_t *) realloc (this_, sizeof (stack_t) + (this_ -> capacity + 1) * sizeof (elem_t));

    if (this_ == nullptr)
    {
        return false;
    }

    for (int i = (this_ -> size); i < (this_ -> capacity); i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    (this_ -> arr) [(this_ -> capacity)] = C_saver_right;

    return true;
}

//=============================================================================

bool Stack_Push (stack_t* this_, elem_t elem)
{
    if ((this_ -> size) >= (this_ -> capacity))
    {
        if (!Stack_Realloc (this_))
        {
            return false;
        }
    }

    (this_ -> arr) [(this_ -> size)++] = elem;

    return true;
}

//=============================================================================

bool Stack_Pop (stack_t* this_, elem_t *elem)
{
    if (Stack_Empty (this_))
    {
        DUMP(Pop_out_of_range, __LINE__)
    }

    *elem = (this_ -> arr) [--(this_ -> size)];
    (this_ -> arr) [(this_ -> size)] = C_poison;

    return true;
}

//=============================================================================

bool Stack_Empty (const stack_t* const this_)
{
    return ((this_ -> size) <= 0);
}

//=============================================================================

elem_t Stack_Top (const stack_t* const this_)
{
    if (Stack_Empty(this_))
    {
        CH(Green)
        printf ("\nStack is empty! There is no top element!\n");
        printf ("So, we give you ");
        PR_B(POISON, Red)
        printf ("!\n");
        return C_poison;
    }

    return (this_ -> arr)[(this_ -> size) - 1];
}

//=============================================================================

int Stack_Size (const stack_t* const this_)
{
    return (this_ -> size);
}

//=============================================================================

bool Stack_Full (const stack_t* const this_)
{
    return ((this_ -> size) == (this_ -> capacity));
}

//=============================================================================

bool Dump (const stack_t* const this_, const int line, const int error, const char * error_name)
{
    printf ("\n");

    /* ERROR IN LINE = [__LINE__] */
    PR_B(ERROR IN LINE, White)
    printf (" = [");
    CH(Yellow)
    printf ("%d", line);
    CH_S
    printf ("]\n");

    /* stack_t '%s' = [0x%p] */
    printf ("stack_t '");
    CH(White)
    printf ("%s", (this_ -> name));
    CH_S
    printf ("' = [");
    CH(Yellow)
    printf ("0x%p", this_);
    CH_S
    printf ("]\n");

    printf ("{\n\t");

    /* ERROR = %d (%s) */
    PR_B(ERROR, Red)
    printf(" = ");
    CH(Yellow)
    printf ("%d ", error);
    printf ("(");
    CH(Green)
    printf ("%s", error_name);
    CH_S
    printf (")\n");

    /* check left saver */
    if ((this_ -> saver_left) != C_saver_left)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("\n\t");
        PR_B(!!!!!!!!, D_red)
        PR_B(ERROR, Red)
        PR_B(!!!!!!!!, D_red)
        printf ("\n");

        printf ("\tSOMETHING CHANGED LEFT SAVER\n");
        printf ("\tLeft saver = [%d], but accept [%d]\n", (this_ -> saver_left), C_saver_left);

        return false;
    }

    if ((this_ -> arr) [(this_ -> capacity)] != C_saver_right)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("\n\t");

        PR_B(!!!!!!!!, D_red)
        PR_B(ERROR, Red)
        PR_B(!!!!!!!!, D_red)
        printf ("\n");

        printf ("\tSOMETHING CHANGED RIGHT SAVER\n");
        printf ("\tRight saver = [%d], but accept [%d]\n", (this_ -> arr) [(this_ -> capacity)], C_saver_right);

        return false;
    }

    printf ("\tSize = %d\n", (this_ -> size));
    /* arr[%d] = [0x%p] */
    printf ("\tarr[%d] = [", (this_ -> capacity) - 1);
    CH(Yellow)
    printf ("0x%p", (this_ -> arr));
    CH_S
    printf ("]\n");

    /* Pointer is near zero! */
    if ((this_ -> arr) == nullptr || (this_ == (stack_t*)C_error_null_ptr))
    {
        CH(Red)
        printf ("\tPointer is near zero!");
        CH_S

        return false;
    }

    printf ("\t{\n");
    for (int i = 0; i < (this_ -> capacity); i++)
    {
        printf ("\t");
        if (i <= (this_ -> size))
        {
            /* [%d]: %d */
            CH(Purple)
            printf ("*");
            CH_S
            printf ("[%02d]: ", i);
            CH(White)
            std::cout << (this_ -> arr) [i];
//            printf ("%d ", (this_ -> arr)[i]);

            CH_S

            if ((int)(this_ -> arr) [i] == C_poison)
            {
                BRACKETS(|, |, Be careful - poison!, Red)
            }
        }
        else
        {
            if ((int)(this_ -> arr) [i] != C_poison)
            {
                printf ("\t\n-----------------------------------------\n");
                printf ("\tERROR!!!:\n");
                printf ("\tarr[%d] is NOT poison!!!\n", i);
                printf ("\ti = [%d], arr[i] = [", i);
                std::cout << (this_ -> arr) [i];
                printf ("], poison = %d\n", C_poison);
//                printf ("\ti = [%d], arr[i] = [%d], poison = %d\n", i, (this_ -> arr)[i], C_poison);
                printf ("-----------------------------------------\n");

                return false;
            }
            else
            {
                /* [%d]: %d |poison| */
                printf (" [%02d]: ", i);
                CH(White)
                std::cout << (this_ -> arr) [i];
//                printf ("%d ", (this_ -> arr)[i]);
                CH_S
                BRACKETS(|, |, poison, Red)
            }
        }
        printf ("\n");
    }
    printf ("\t}\n");
    printf ("}\n");

    return false;
}
