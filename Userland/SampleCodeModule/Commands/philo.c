// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <philo.h>

typedef struct {
	char state;
	int pid;
} Philosopher;

static Philosopher philosophers[MAX_PHILOSOPHERS];
static int amount_of_philosophers = 0;
static char *mutex = "mtx";
static char *change_philo = "chng";
static int16_t fds[3] = {STDIN, STDOUT, STDERR};
static int16_t philosopher_fds[3] = {DEV_NULL, STDOUT, STDERR};

static void can_eat(int id) {
	int left = (id + amount_of_philosophers - 1) % amount_of_philosophers;
	int right = (id + 1) % amount_of_philosophers;
	if (philosophers[left].state != EATING && philosophers[right].state != EATING) {
		philosophers[id].state = EATING;
		call_sys_sem_post(my_itoa(id));
	}
}

static void grab_forks(int id) {
	call_sys_sem_wait(mutex);
	can_eat(id);
	call_sys_sem_post(mutex);

	call_sys_sem_wait(my_itoa(id));
}

static void release_forks(int id) {
	int left = (id + amount_of_philosophers - 1) % amount_of_philosophers;
	int right = (id + 1) % amount_of_philosophers;
	call_sys_sem_wait(mutex);
	philosophers[id].state = THINKING;
	can_eat(left);
	can_eat(right);
	call_sys_sem_post(mutex);
}

static void print_table_state() {
	for (int i = 0; i < amount_of_philosophers; i++) {
		if (philosophers[i].state == EATING) {
			printf("%c", 'E');
		}
		else {
			printf("%c", '.');
		}
		printf(" ");
	}
	printf("\n");
}

void philosopher_process(int argc, char **argv) {
	int id = my_atoi(argv[0]);

	while (1) {
		call_sys_sleep(THINKING_TIME);

		grab_forks(id);
		call_sys_sleep(EATING_TIME);

		release_forks(id);
		print_table_state();
	}
}

static char add_philosopher() {
	call_sys_sem_wait(change_philo);
	if (amount_of_philosophers >= MAX_PHILOSOPHERS) {
		call_sys_error("Error: max philosophers reached\n", STDERR);
		call_sys_sem_post(change_philo);
		return -1;
	}

	int id = amount_of_philosophers;
	amount_of_philosophers++;
	philosophers[id].state = THINKING;

	call_sys_sem_open(my_itoa(id), 1);

	char *args[1] = {my_itoa(id)};
	philosophers[id].pid = call_sys_create_process("Philosopher", 4, args, 1, &philosopher_process, philosopher_fds);

	call_sys_sem_post(change_philo);
	return 0;
}

static char delete_philosopher() {
	call_sys_sem_wait(change_philo);
	int i = amount_of_philosophers - 1;
	call_sys_sem_close(my_itoa(i));
	call_sys_kill_process(philosophers[i].pid);
	call_sys_wait_children(philosophers[i].pid);
	amount_of_philosophers--;
	call_sys_sem_post(change_philo);
	return 0;
}

void table_manager() {
	char input;
	while ((input = get_char()) != QUIT) {
		switch (input) {
			case ADD:
				add_philosopher();
				printf("Added philosopher\n");
				break;
			case REMOVE:
				delete_philosopher();
				printf("Removed philosopher\n");
				break;
			default:
				break;
		}
	}

	int actual_philo = amount_of_philosophers;
	for (int i = 0; i < actual_philo; i++) {
		delete_philosopher();
	}

	call_sys_sem_close(mutex);
	call_sys_sem_close(change_philo);
	call_sys_enter();
}

static void header() {
	printf("Press 'a' to ADD a philosopher\n");
	printf("Press 'r' to REMOVE a philosopher\n");
	printf("Press 'q' to QUIT\n");
	printf("----------------------------------\n");
}

int initialize_philosophers() {
	call_sys_sem_open(mutex, 1);
	call_sys_sem_open(change_philo, 1);

	int parent_pid = call_sys_create_process("Table Manager", 4, NULL, 0, &table_manager, fds);
	if (parent_pid == -1) {
		call_sys_error("Error: creating table manager\n", STDERR);
		return -1;
	}
	header();

	for (int i = 0; i < MIN_PHILOSOPHERS; i++) {
		add_philosopher();
	}
	return parent_pid;
}

int philo(int16_t fds[]) {
	int parent_pid = initialize_philosophers();
	call_sys_wait_children(parent_pid);
	return parent_pid;
}
