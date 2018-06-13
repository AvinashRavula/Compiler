void clear_screen()
{
	system("cls");
}

void press_any_key()
{
	printf("\nPress any key to return to previous menu.");
	getch();
}

char *prompt(char *message)
{
	fflush(stdin);
	printf("\n%s", message);
	char *input = (char *)malloc(sizeof(char) * 50);
	gets(input);
	return input;
}
