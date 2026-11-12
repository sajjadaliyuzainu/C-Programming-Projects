/* Computer Laboratory Equipment Inventory Management System
   Stage 2: Equipment structure, add equipment, view all equipment */

#include <stdio.h>
#include <string.h>

#define MAX_EQUIPMENT 100
#define NAME_LEN 50
#define CATEGORY_LEN 30
#define DATA_FILE "equipment.dat"

typedef struct {
    int id;
    char name[NAME_LEN];
    char category[CATEGORY_LEN];
    int quantity;
    int status; /* 1 = Working, 2 = Faulty, 3 = Under repair */
} Equipment;

void clear_input_buffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* keep reading and throwing away characters */
    }
}

int read_int(const char *prompt, int min, int max)
{
    int value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        clear_input_buffer();

        if (result == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a whole number from %d to %d.\n", min, max);
    }
}

void read_string(const char *prompt, char *dest, int size)
{
    size_t len;

    while (1) {
        printf("%s", prompt);

        if (fgets(dest, size, stdin) == NULL) {
            snprintf(dest, size, "Unknown");
            return;
        }

        len = strlen(dest);
        if (len > 0 && dest[len - 1] == '\n') {
            dest[len - 1] = '\0';
        } else {
            clear_input_buffer();
        }

        if (dest[0] != '\0') {
            return;
        }
        printf("This field cannot be empty.\n");
    }
}

const char *status_to_text(int status)
{
    switch (status) {
        case 1: return "Working";
        case 2: return "Faulty";
        case 3: return "Under repair";
        default: return "Unknown";
    }
}

int find_index_by_id(const Equipment list[], int count, int id)
{
    int i;

    for (i = 0; i < count; i++) {
        if (list[i].id == id) {
            return i;
        }
    }
    return -1;
}

void show_menu(void)
{
    printf("\n==============================================\n");
    printf("  COMPUTER LABORATORY EQUIPMENT INVENTORY\n");
    printf("              MANAGEMENT SYSTEM\n");
    printf("==============================================\n");
    printf("  1. Add equipment\n");
    printf("  2. View all equipment\n");
    printf("  3. Search equipment by ID\n");
    printf("  4. Update equipment\n");
    printf("  5. Delete equipment\n");
    printf("  6. View inventory summary\n");
    printf("  0. Exit\n");
    printf("----------------------------------------------\n");
    printf("Enter your choice: ");
}

void add_equipment(Equipment list[], int *count)
{
    Equipment item;

    if (*count >= MAX_EQUIPMENT) {
        printf("\nInventory is full. Cannot add more equipment.\n");
        return;
    }

    printf("\n--- Add Equipment ---\n");

    item.id = read_int("Enter equipment ID (1 - 9999): ", 1, 9999);
    if (find_index_by_id(list, *count, item.id) != -1) {
        printf("\nAn item with ID %d already exists.\n", item.id);
        return;
    }

    read_string("Enter equipment name: ", item.name, NAME_LEN);
    read_string("Enter category (e.g. Computer, Network): ", item.category, CATEGORY_LEN);
    item.quantity = read_int("Enter quantity (1 - 1000): ", 1, 1000);
    printf("Status options: 1 = Working, 2 = Faulty, 3 = Under repair\n");
    item.status = read_int("Enter status (1 - 3): ", 1, 3);

    list[*count] = item;
    (*count)++;

    printf("\nEquipment added successfully.\n");
}

void view_all_equipment(const Equipment list[], int count)
{
    int i;

    if (count == 0) {
        printf("\nNo equipment records yet.\n");
        return;
    }

    printf("\n%-6s %-25s %-15s %-8s %-12s\n",
           "ID", "Name", "Category", "Qty", "Status");
    printf("----------------------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-6d %-25.25s %-15.15s %-8d %-12s\n",
               list[i].id,
               list[i].name,
               list[i].category,
               list[i].quantity,
               status_to_text(list[i].status));
    }

    printf("\nTotal records: %d\n", count);
}
void print_equipment_details(const Equipment *item)
{
    printf("\n--- Equipment Details ---\n");
    printf("ID       : %d\n", item->id);
    printf("Name     : %s\n", item->name);
    printf("Category : %s\n", item->category);
    printf("Quantity : %d\n", item->quantity);
    printf("Status   : %s\n", status_to_text(item->status));
}

void search_equipment(const Equipment list[], int count)
{
    int id;
    int index;

    if (count == 0) {
        printf("\nNo equipment records yet.\n");
        return;
    }

    printf("\n--- Search Equipment ---\n");
    id = read_int("Enter equipment ID to search (1 - 9999): ", 1, 9999);
    index = find_index_by_id(list, count, id);

    if (index == -1) {
        printf("\nNo equipment found with ID %d.\n", id);
        return;
    }

    print_equipment_details(&list[index]);
}

void update_equipment(Equipment list[], int count)
{
    int id;
    int index;
    int choice;
    Equipment *item;

    if (count == 0) {
        printf("\nNo equipment records yet.\n");
        return;
    }

    printf("\n--- Update Equipment ---\n");
    id = read_int("Enter ID of equipment to update (1 - 9999): ", 1, 9999);
    index = find_index_by_id(list, count, id);

    if (index == -1) {
        printf("\nNo equipment found with ID %d.\n", id);
        return;
    }

    item = &list[index];

    do {
        print_equipment_details(item);
        printf("\nWhat do you want to update?\n");
        printf("  1. Name\n");
        printf("  2. Category\n");
        printf("  3. Quantity\n");
        printf("  4. Status\n");
        printf("  0. Done\n");
        choice = read_int("Enter choice (0 - 4): ", 0, 4);

        switch (choice) {
            case 1:
                read_string("Enter new name: ", item->name, NAME_LEN);
                printf("Name updated.\n");
                break;
            case 2:
                read_string("Enter new category: ", item->category, CATEGORY_LEN);
                printf("Category updated.\n");
                break;
            case 3:
                item->quantity = read_int("Enter new quantity (1 - 1000): ", 1, 1000);
                printf("Quantity updated.\n");
                break;
            case 4:
                printf("Status options: 1 = Working, 2 = Faulty, 3 = Under repair\n");
                item->status = read_int("Enter new status (1 - 3): ", 1, 3);
                printf("Status updated.\n");
                break;
            case 0:
                printf("\nUpdate finished.\n");
                break;
        }
    } while (choice != 0);
}int confirm(const char *prompt)
{
    char answer[10];

    read_string(prompt, answer, sizeof answer);
    return (answer[0] == 'y' || answer[0] == 'Y');
}

void delete_equipment(Equipment list[], int *count)
{
    int id;
    int index;
    int i;

    if (*count == 0) {
        printf("\nNo equipment records yet.\n");
        return;
    }

    printf("\n--- Delete Equipment ---\n");
    id = read_int("Enter ID of equipment to delete (1 - 9999): ", 1, 9999);
    index = find_index_by_id(list, *count, id);

    if (index == -1) {
        printf("\nNo equipment found with ID %d.\n", id);
        return;
    }

    print_equipment_details(&list[index]);

    if (!confirm("\nAre you sure you want to delete this record? (y/n): ")) {
        printf("\nDelete cancelled.\n");
        return;
    }

    for (i = index; i < *count - 1; i++) {
        list[i] = list[i + 1];
    }
    (*count)--;

    printf("\nEquipment deleted.\n");
}

void view_summary(const Equipment list[], int count)
{
    int i;
    int total_units = 0;
    int working = 0;
    int faulty = 0;
    int repair = 0;

    if (count == 0) {
        printf("\nNo equipment records yet.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        total_units += list[i].quantity;

        switch (list[i].status) {
            case 1: working += list[i].quantity; break;
            case 2: faulty += list[i].quantity; break;
            case 3: repair += list[i].quantity; break;
        }
    }

    printf("\n=========== INVENTORY SUMMARY ===========\n");
    printf("Equipment records   : %d\n", count);
    printf("Total units         : %d\n", total_units);
    printf("-----------------------------------------\n");
    printf("Working units       : %d\n", working);
    printf("Faulty units        : %d\n", faulty);
    printf("Under repair units  : %d\n", repair);
    printf("-----------------------------------------\n");
    printf("Percentage working  : %.1f%%\n", (working * 100.0) / total_units);
    printf("=========================================\n");
}int save_equipment(const Equipment list[], int count)
{
    FILE *fp = fopen(DATA_FILE, "wb");

    if (fp == NULL) {
        printf("\nError: could not open the data file for saving.\n");
        return 0;
    }

    if (fwrite(&count, sizeof count, 1, fp) != 1 ||
        fwrite(list, sizeof(Equipment), count, fp) != (size_t)count) {
        printf("\nError: could not write all data to the file.\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

int load_equipment(Equipment list[], int *count)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    int stored;

    *count = 0;

    if (fp == NULL) {
        return 1; /* no file yet: this is normal on the first run */
    }

    if (fread(&stored, sizeof stored, 1, fp) != 1 ||
        stored < 0 || stored > MAX_EQUIPMENT ||
        fread(list, sizeof(Equipment), stored, fp) != (size_t)stored) {
        fclose(fp);
        return 0; /* file is damaged or unreadable */
    }

    fclose(fp);
    *count = stored;
    return 1;
}
int main(void)
{
    Equipment inventory[MAX_EQUIPMENT];
    int count = 0;
    int choice;    if (!load_equipment(inventory, &count)) {
        printf("Warning: the data file could not be read. Starting empty.\n");
    } else if (count > 0) {
        printf("Loaded %d record(s) from file.\n", count);
    }

    do {
        show_menu();

        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        clear_input_buffer();

        switch (choice) {
              case 1:
                add_equipment(inventory, &count);
                save_equipment(inventory, count);
                break;
            case 2:
                view_all_equipment(inventory, count);
                break;
             case 3:
                search_equipment(inventory, count);
                break;
             case 4:
                update_equipment(inventory, count);
                save_equipment(inventory, count);
                break;
            case 5:
                delete_equipment(inventory, &count);
                save_equipment(inventory, count);
                break;
            case 6:
                view_summary(inventory, count);
                break;
            case 0:
                printf("\nThank you for using the system. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please enter a number from 0 to 6.\n");
        }
    } while (choice != 0);

    return 0;
}