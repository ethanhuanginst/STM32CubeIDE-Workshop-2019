#ifndef OPTIONS_H
#define OPTIONS_H

enum option_ids {
    OPTION_ID_TGEN_MODE,
    OPTION_ID_TGEN_MODE_FIXED,
    OPTION_ID_TGEN_MODE_TRACK,
    OPTION_ID_TGEN_MODE_OFF,
    OPTION_ID_TGEN_FREQ,
    OPTION_ID_TGEN_ADJ,

    OPTION_ID_VIEW_FDISP,
    OPTION_ID_VIEW_FDISP_CENTERSPAN,
    OPTION_ID_VIEW_FDISP_STARTSTOP,
    OPTION_ID_VIEW_WATERFALL,
    OPTION_ID_VIEW_WATERFALL_ON,
    OPTION_ID_VIEW_WATERFALL_OFF,
    OPTION_ID_VIEW_NORMALISE,
};

enum option_types {
    OPTION_TYPE_SELECTION,
    OPTION_TYPE_FREQUENCY,
    OPTION_TYPE_ACTION,
};

#define MAX_CATEGORIES 10
#define MAX_OPTIONS_IN_CATEGORY 30
#define MAX_SELECTIONS_IN_OPTION 6

typedef struct _option {
    char *name;
    int id;
    int type;
    int enable;
    void *data;
} option_t;

typedef struct _category {
    char *name;
    option_t options[MAX_OPTIONS_IN_CATEGORY];
    int nOptions;
} category_t;

typedef struct _menu {
    category_t categories[MAX_CATEGORIES];
    int nCategories;
} menu_t;

typedef struct _selection {
    char *name;
    int id;
} selection_t;

typedef struct _selection_option {
    selection_t selections[MAX_SELECTIONS_IN_OPTION];
    int nSelections;
    int value;
} selection_option_t;

typedef void (*action_callback_fptr)(void); 

// Populate options
void options_init();

// Call after options are changed as some options depend on others
void options_refresh();

category_t *option_add_category(char *name);

void option_add_selection(category_t *target, int id, char *name, selection_t *values, int nValues, int init);

void option_add_frequency(category_t *target, int id, char *name, double init);

void option_add_action(category_t *target, int id, char *name, void (*callback)(void) );

void option_enable(int id);

void option_disable(int id);

menu_t *get_option_menu();

option_t *option_get_generic(int id);

int option_get_selection(int id);

double option_get_frequency(int id);

#endif
