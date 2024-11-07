#include <Phylo.h>

static char state[MAX_PHILOSOPHERS];
static int amount_of_philosophers = 0;
static int philosopher_pids[MAX_PHILOSOPHERS]; 
static char fork_sem_names[MAX_PHILOSOPHERS][5]; 
int16_t fds[3] = {STDIN, STDOUT, STDERR};
int16_t philosopher_fds[3] = {DEV_NULL, STDOUT, STDERR};

static char add_philosopher();
static char delete_philosoper();

static char *dining_room_sem = "dining_room";

void philosopher_process(int argc, char **argv) {
    char left_fork[5], right_fork[5];
    int id = my_atoi(argv[0]);
    strcpy(left_fork, fork_sem_names[id]);
    strcpy(right_fork, fork_sem_names[(id + 1) % amount_of_philosophers]);

    while (1) {
        state[id] = THINKING;
        print_table_state();
        call_sys_sleep(2 + (id % 3)); 

        call_sys_sem_wait(dining_room_sem);
        
        call_sys_sem_wait(left_fork);
        
        call_sys_sem_wait(right_fork);

        state[id] = EATING;
        
        print_table_state();
        call_sys_sleep(3 + (id % 2)); 


        call_sys_sem_post(right_fork);
        call_sys_sem_post(left_fork);

        call_sys_sem_post(dining_room_sem);
    }
}

void print_table_state() {
    for (int i = 0; i < amount_of_philosophers; i++) {
        call_sys_drawChar(state[i]);
        call_sys_drawChar(' ');
    }
    call_sys_drawChar('\n');
}

int initialize_philosophers() {
    call_sys_sem_open(dining_room_sem, MIN_PHILOSOPHERS - 1);

    int parent_pid = call_sys_create_process("Table Manager", 4, NULL, 0, &table_manager, fds);
    if(parent_pid == -1){
        call_sys_error("Error: creating table manager\n", STDERR);
        return -1;
    }
    for (int i = 0; i < MIN_PHILOSOPHERS; i++) {
        add_philosopher();
    }
    return parent_pid;
}

static char add_philosopher(){
    amount_of_philosophers++;
    strcpy(fork_sem_names[amount_of_philosophers - 1], my_itoa(amount_of_philosophers - 1));

    if(call_sys_sem_open(fork_sem_names[amount_of_philosophers - 1], 1) == -1){
        call_sys_error("Error: creating fork semaphore\n", STDERR);
        return;
    }

    char *args[1] = {my_itoa(amount_of_philosophers - 1)};
    philosopher_pids[amount_of_philosophers - 1] = call_sys_create_process("Philosopher", 4, args, 1, &philosopher_process, philosopher_fds);

    if(philosopher_pids[amount_of_philosophers - 1] == -1){
        call_sys_error("Error: creating philosopher\n", STDERR);
        amount_of_philosophers--;
        return -1;
    }
    state[amount_of_philosophers - 1] = THINKING;
    return 0;
}

static char delete_philosoper(){
    amount_of_philosophers--;
    if(call_sys_kill_process(philosopher_pids[amount_of_philosophers]) == -1){
        call_sys_error("Error: killing philosopher\n", STDERR);
        amount_of_philosophers++;
        return -1;
    }

    if(call_sys_wait_children(philosopher_pids[amount_of_philosophers]) == -1){
        call_sys_error("Error: waiting for philosopher\n", STDERR);
        return -1;
    }

    philosopher_pids[amount_of_philosophers] = 0;
    call_sys_sem_close(fork_sem_names[amount_of_philosophers]);
    return 0;
}

void reset_philosophers(int actual_philo) {
    // Reset all static variables to initial states
    for (int i = 0; i < actual_philo; i++) {
        state[i] = THINKING;
        philosopher_pids[i] = 0;
        fork_sem_names[i][0] = '\0';
    }

}


void table_manager() {
    printf("Press 'a' to ADD a philosopher\n");
    printf("Press 'r' to REMOVE a philosopher\n");
    printf("Press 'q' to QUIT\n");
    printf("----------------------------------\n");
    char input;
    while ((input = getChar()) != QUIT) {
        if (input == ADD && amount_of_philosophers < MAX_PHILOSOPHERS) {
            if(add_philosopher() == -1){
                break;
            }
            call_sys_drawWord("Added a philosopher\n");
        } else if (input == REMOVE && amount_of_philosophers > MIN_PHILOSOPHERS) {
            if(delete_philosoper()== -1){
                break;
            }
            call_sys_drawWord("Removed a philosopher\n");
        }
    }
    

    int actual_philo = amount_of_philosophers;
    for(int i=0; i<actual_philo; i++){
        delete_philosoper();
    }
    call_sys_sem_close(dining_room_sem);
    reset_philosophers(actual_philo);
    call_sys_enter();
}

int philo(int16_t fds[]) {
    int parent_pid = initialize_philosophers();
    return parent_pid;
}
