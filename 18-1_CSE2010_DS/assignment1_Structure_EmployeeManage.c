#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//using namespace std;

int number;

typedef struct employee {
	char name[10];
	int age;
	int salary;
	char department[20];
} Employee;

Employee Empty = { 0,0,0,0 };

void insert(Employee *people, char *word) {
	int i;

	for (i = 0; i < number; i++) {
		if (strcmp(people[i].name, "") == 0) break;
	}
	if (i == number) {
		printf("Not enough space\n");
		return;
	}

	word = strtok(NULL, " ");
	strcpy(people[i].name, word);

	word = strtok(NULL, " ");
	people[i].age = atoi(word);

	word = strtok(NULL, " ");
	people[i].salary = atoi(word);

	word = strtok(NULL, " ");
	strcpy(people[i].department, word);
}

void delete_one(Employee *people, char *word) {
	word = strtok(NULL, " ");

	int i;
	for (i = 0; i < number; i++) {
		if (strcmp(people[i].name, word) == 0) {
			people[i] = Empty;
			return;
		}
	}
	if (i == number) {
		printf("Not found\n");
		return;
	}
}

int find(Employee *people, char *word) {
	word = strtok(NULL, " ");

	int i;
	for (i = 0; i < number; i++) {
		if (strcmp(people[i].name, word) == 0) return i;
	}
	if (i == number) return -1;
}

void inform(Employee *people, char *word) {
	word = strtok(NULL, " ");

	int i;
	int ans = 0;
	for (i = 0; i < number; i++) {
		if (strcmp(people[i].name, word) == 0) {
			printf("%s %d %d %s\n", people[i].name, people[i].age, people[i].salary, people[i].department);
			ans = 1;
		}
	}
	if (i == number&&ans == 0) printf("Not found\n");
}

int avgBenefit(Employee *people, char *word) {
	word = strtok(NULL, " ");

	int i;
	int sum = 0;
	int num = 0;
	for (i = 0; i < number; i++) {
		if (strcmp(people[i].department, word) == 0) {
			sum += people[i].salary;
			num++;
		}
	}
	if (num == 0) return 0;
	else return sum / num;
}

int main(void) {
	char input[100];
	char *word;
	Employee *people;
	int flag = 0, i;

	gets_s(input, sizeof(input));
	word = strtok(input, " ");

	if (word == NULL) return 0;

	if (strcmp(word, "set") == 0) {
		word = strtok(NULL, " ");
		number = atoi(word);
		people = (Employee*)malloc(sizeof(Employee)*number);
		flag = 1;
	}
	if (!flag) return 0;


	for (i = 0; i < number; i++) people[i] = Empty;

	while (1) {
		gets_s(input, sizeof(input));
		word = strtok(input, " ");

		if (word == NULL) break;

		if (strcmp(word, "insert") == 0) insert(people, word);
		else if (strcmp(word, "delete") == 0) delete_one(people, word);
		else if (strcmp(word, "find") == 0) printf("%d\n", find(people, word));
		else if (strcmp(word, "inform") == 0) inform(people, word);
		else if (strcmp(word, "avg") == 0) printf("%d\n", avgBenefit(people, word));
	}

	free(people);
	return 0;
}