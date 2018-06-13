
bool duplicate_variable_check(char *);

void printErrorWithNullInputs()
{
	//printf("\nNo Records\n");
}

void printErrorProcessingHashCode()
{
	printf("\nError generating hash code\n");
}


int sumOfChars(char *str)
{
	int len = strlen(str);
	int sum = 0;
	for (int i = 0; i < len; i++)
	{
		sum += str[i];
	}
	return sum;
}

// returns -1 if NULL input is given else returns the generated hashCode with key and size
int hashCode(char *key, int size) {
	if (key == NULL)
		return -1;
	return sumOfChars(key) % size;
}


struct op_item *searchInOpTable(char *key) {

	if (main_op_table.hashTableCount < 1 || key == NULL)
	{
		printErrorWithNullInputs();
		return NULL;
	}
	int hashIndex = hashCode(key, main_op_table.hashTableSize);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return NULL;
	}
	struct op_item *item = main_op_table.item[hashIndex];
	while (item != NULL) {
		if (strcmp(item->op_name, key) == 0)
			return item;
		item = item->next;
	}
	return NULL;
}

bool insertIntoOpTable(char *op_name, int op_code) {
	if (main_op_table.hashTableCount < 0 || op_name == NULL )
	{
		printErrorWithNullInputs();
		return false;
	}

	op_item *item = (struct op_item*) malloc(sizeof(struct op_item));
	strcpy(item->op_name, op_name);
	item->op_code = op_code;
	item->next = NULL;
	int hashIndex = hashCode(op_name, main_op_table.hashTableSize);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}

	if (main_op_table.item[hashIndex] != NULL) {
		op_item *tItem = main_op_table.item[hashIndex];
		if (strcmp(tItem->op_name, op_name) == 0)
		{
			item->next = main_op_table.item[hashIndex]->next;
			main_op_table.item[hashIndex] = item;
			main_op_table.hashTableCount++;
			return true;
		}
		while (tItem->next != NULL)
		{
			tItem = tItem->next;
		}
		tItem->next = item;
		main_op_table.hashTableCount++;
		return true;
	}

	main_op_table.item[hashIndex] = item;
	main_op_table.hashTableCount++;
	return true;
}

bool deleteInOpTable( char *op_name) {
	if (op_name == NULL || main_op_table.hashTableCount < 1)
	{
		printErrorWithNullInputs();
		return false;
	}
	char * key = op_name;
	int hashIndex = hashCode(key, main_op_table.hashTableSize);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}
	op_item *item = main_op_table.item[hashIndex];
	op_item *prevItem = item;
	while (item->next != NULL) {
		if (strcmp(main_op_table.item[hashIndex]->op_name, key) == 0) {
			op_item* temp = main_op_table.item[hashIndex];
			prevItem->next = item->next;
			free(temp);
			return true;
		}
		prevItem = item;
		item = item->next;
	}
	main_op_table.item[hashIndex] = NULL;
	free(item);
	return true;;
}


void displayOpTable() {
	clear_screen();
	if (main_op_table.hashTableCount < 1)
	{
		printErrorWithNullInputs();
		press_any_key();
		return;
	}
	int i = 0;
	for (i = 0; i<main_op_table.hashTableSize; i++) {
		if (main_op_table.item[i] != NULL){
			printf("\n%10s %10d",main_op_table.item[i]->op_name,main_op_table.item[i]->op_code);
			op_item *item = main_op_table.item[i]->next;
			while (item != NULL)
			{
				printf("\n%10s %10d", item->op_name, item->op_code);
				item = item->next;
			}
		}
	}
	press_any_key();
}



bool insertIntoConstantsSymbolTable(char *var_name, int start_address, int size) {
	if (main_symbol_table.count < 0 || var_name == NULL)
	{
		printErrorWithNullInputs();
		return false;
	}
	
	if (duplicate_variable_check(var_name))
		return false;

	constant_symbol_item *item = (constant_symbol_item*)malloc(sizeof(constant_symbol_item));
	item->var_name = (char *)malloc(sizeof(char) * (strlen(var_name) + 1));
	strcpy(item->var_name, var_name);
	item->start_address = start_address;
	item->size = size;
	item->next = NULL;
	int hashIndex = hashCode(var_name, main_constant_symbol_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}

	if (main_constant_symbol_table.item[hashIndex] != NULL) {
		constant_symbol_item *tItem = main_constant_symbol_table.item[hashIndex];
		if (strcmp(tItem->var_name, var_name) == 0)
		{
			item->next = main_constant_symbol_table.item[hashIndex]->next;
			main_constant_symbol_table.item[hashIndex] = item;
			main_constant_symbol_table.count++;
			return true;
		}
		while (tItem->next != NULL)
		{
			tItem = tItem->next;
		}
		tItem->next = item;
		main_constant_symbol_table.count++;
		return true;
	}

	main_constant_symbol_table.item[hashIndex] = item;
	main_constant_symbol_table.count++;
	return true;
}


bool deleteInConstantSymbolTable(char *var_name) {
	if (var_name == NULL || main_constant_symbol_table.count < 1)
	{
		printErrorWithNullInputs();
		return false;
	}
	char * key = var_name;
	int hashIndex = hashCode(key, main_constant_symbol_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}
	constant_symbol_item *item = main_constant_symbol_table.item[hashIndex];
	constant_symbol_item *prevItem = item;
	while (item->next != NULL) {
		if (strcmp(main_op_table.item[hashIndex]->op_name, key) == 0) {
			constant_symbol_item* temp = main_constant_symbol_table.item[hashIndex];
			prevItem->next = item->next;
			free(temp);
			return true;
		}
		prevItem = item;
		item = item->next;
	}
	main_constant_symbol_table.item[hashIndex] = NULL;
	free(item);
	return true;;
}


void displayConstantSymbolTable() {
	clear_screen();
	if (main_constant_symbol_table.count < 1)
	{
		printErrorWithNullInputs();
		press_any_key();
		return;
	}
	int i = 0;
	for (i = 0; i<main_constant_symbol_table.size; i++) {
		if (main_symbol_table.item[i] != NULL){
			printf("\n%10s %10d %10d", main_constant_symbol_table.item[i]->var_name, main_constant_symbol_table.item[i]->start_address, main_constant_symbol_table.item[i]->size);
			constant_symbol_item *item = main_constant_symbol_table.item[i]->next;
			while (item != NULL)
			{
				printf("%10s %10d %10d", item->var_name, item->start_address, item->size);
				item = item->next;
			}
		}
	}
	press_any_key();
}

struct constant_symbol_item *searchInConstantSymbolTable(char *key) {

	if (main_constant_symbol_table.count < 1 || key == NULL)
	{
		printErrorWithNullInputs();
		return NULL;
	}
	int hashIndex = hashCode(key, main_constant_symbol_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return NULL;
	}
	struct constant_symbol_item *item = main_constant_symbol_table.item[hashIndex];
	while (item != NULL) {
		if (strcmp(item->var_name, key) == 0)
			return item;
		item = item->next;
	}
	return NULL;
}

bool insertIntoSymbolTable(char *var_name, int start_address, int size) {
	if (main_symbol_table.count < 0 || var_name == NULL)
	{
		printErrorWithNullInputs();
		return false;
	}

	if (duplicate_variable_check(var_name))
		return false;

	symbol_item *item = (symbol_item*) malloc(sizeof(symbol_item));
	item->var_name = (char *)malloc(sizeof(char) * (strlen(var_name) + 1));
	strcpy(item->var_name, var_name);
	item->start_address = start_address;
	item->size = size;
	item->next = NULL;
	int hashIndex = hashCode(var_name, main_symbol_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}

	if (main_symbol_table.item[hashIndex] != NULL) {
		symbol_item *tItem = main_symbol_table.item[hashIndex];
		if (strcmp(tItem->var_name, var_name) == 0)
		{
			item->next = main_symbol_table.item[hashIndex]->next;
			main_symbol_table.item[hashIndex] = item;
			main_symbol_table.count++;
			return true;
		}
		while (tItem->next != NULL)
		{
			tItem = tItem->next;
		}
		tItem->next = item;
		main_symbol_table.count++;
		return true;
	}

	main_symbol_table.item[hashIndex] = item;
	main_symbol_table.count++;
	return true;
}

bool deleteInSymbolTable(char *var_name) {
	if (var_name == NULL || main_symbol_table.count < 1)
	{
		printErrorWithNullInputs();
		return false;
	}
	char * key = var_name;
	int hashIndex = hashCode(key, main_symbol_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}
	symbol_item *item = main_symbol_table.item[hashIndex];
	symbol_item *prevItem = item;
	while (item->next != NULL) {
		if (strcmp(main_symbol_table.item[hashIndex]->var_name, key) == 0) {
			symbol_item* temp = main_symbol_table.item[hashIndex];
			prevItem->next = item->next;
			free(temp);
			return true;
		}
		prevItem = item;
		item = item->next;
	}
	main_symbol_table.item[hashIndex] = NULL;
	free(item);
	return true;;
}


void displaySymbolTable() {
	clear_screen();
	if (main_symbol_table.count < 1)
	{
		printErrorWithNullInputs();
		press_any_key();
		return;
	}
	int i = 0;
	for (i = 0; i<main_symbol_table.size; i++) {
		if (main_symbol_table.item[i] != NULL){
			printf("\n%10s %10d %10d ", main_symbol_table.item[i]->var_name, main_symbol_table.item[i]->start_address, main_symbol_table.item[i]->size);
			symbol_item *item = main_symbol_table.item[i]->next;
			while (item != NULL)
			{
				printf("\n%10s %10d %10d", item->var_name, item->start_address, item->size);
				item = item->next;
			}
		}
	}
	press_any_key();
}

struct symbol_item *searchInSymbolTable(char *key) {

	if (main_symbol_table.count < 1 || key == NULL)
	{
		printErrorWithNullInputs();
		return NULL;
	}
	int hashIndex = hashCode(key, main_symbol_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return NULL;
	}
	struct symbol_item *item = main_symbol_table.item[hashIndex];
	while (item != NULL) {
		if (strcmp(item->var_name, key) == 0)
			return item;
		item = item->next;
	}
	return NULL;
}



bool insertIntoLabelTable(char *label_name, int instruction_number) {
	if (main_label_table.count < 0 || label_name == NULL)
	{
		printErrorWithNullInputs();
		return false;
	}

	if (duplicate_variable_check(label_name))
		return false;

	label_item *item = (label_item*)malloc(sizeof(label_item));
	item->label_name = (char *)malloc(sizeof(char) * (strlen(label_name) + 1));
	strcpy(item->label_name, label_name);
	item->instruction_number = instruction_number;
	item->next = NULL;
	int hashIndex = hashCode(label_name, main_label_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}

	if (main_label_table.item[hashIndex] != NULL) {
		label_item *tItem = main_label_table.item[hashIndex];
		if (strcmp(tItem->label_name, label_name) == 0)
		{
			item->next = main_label_table.item[hashIndex]->next;
			main_label_table.item[hashIndex] = item;
			main_label_table.count++;
			return true;
		}
		while (tItem->next != NULL)
		{
			tItem = tItem->next;
		}
		tItem->next = item;
		main_label_table.count++;
		return true;
	}

	main_label_table.item[hashIndex] = item;
	main_label_table.count++;
	return true;
}

bool deleteInLabelTable(char *label_name) {
	if (label_name == NULL || main_label_table.count < 1)
	{
		printErrorWithNullInputs();
		return false;
	}
	char * key = label_name;
	int hashIndex = hashCode(key, main_label_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return false;
	}
	label_item *item = main_label_table.item[hashIndex];
	label_item *prevItem = item;
	while (item->next != NULL) {
		if (strcmp(main_label_table.item[hashIndex]->label_name, key) == 0) {
			label_item* temp = main_label_table.item[hashIndex];
			prevItem->next = item->next;
			free(temp);
			return true;
		}
		prevItem = item;
		item = item->next;
	}
	main_label_table.item[hashIndex] = NULL;
	free(item);
	return true;;
}


void displayLabelTable() {
	clear_screen();
	if (main_label_table.count < 1)
	{
		printErrorWithNullInputs();
		press_any_key();
		return;
	}
	int i = 0;
	for (i = 0; i<main_label_table.size; i++) {
		if (main_label_table.item[i] != NULL){
			printf("\n%10s %10d ", main_label_table.item[i]->label_name, main_label_table.item[i]->instruction_number);
			label_item *item = main_label_table.item[i]->next;
			while (item != NULL)
			{
				printf("\n%10s %10d", item->label_name, item->instruction_number);
				item = item->next;
			}
		}
	}
	press_any_key();
}

label_item *searchInLabelTable(char *key) {

	if (main_label_table.count < 1 || key == NULL)
	{
		printErrorWithNullInputs();
		return NULL;
	}
	int hashIndex = hashCode(key, main_label_table.size);
	if (hashIndex == -1)
	{
		printErrorProcessingHashCode();
		return NULL;
	}
	label_item *item = main_label_table.item[hashIndex];
	while (item != NULL) {
		if (strcmp(item->label_name, key) == 0)
			return item;
		item = item->next;
	}
	return NULL;
}

bool duplicate_variable_check(char *label_name)
{
	label_item *item = searchInLabelTable(label_name);
	symbol_item *symbl_item = searchInSymbolTable(label_name);
	constant_symbol_item *const_item = searchInConstantSymbolTable(label_name);
	op_item *op_code_item = searchInOpTable(label_name);
	if (item != NULL || symbl_item != NULL || const_item != NULL || op_code_item != NULL)
	{
		printf("\nduplicate  word : %s", label_name);
		return true;
	}
	return false;
}