#include <phylo.h>

typedef struct {
    char state;
    int pid;
    char left_fork[5];
    char right_fork[5];
}Philosopher;

static Philosopher philosophers[MAX_PHILOSOPHERS];

static int amount_of_philosophers = 0;
static char *mutex = "mtx";
static int16_t fds[3] = {STDIN, STDOUT, STDERR};
static int16_t philosopher_fds[3] = {DEV_NULL, STDOUT, STDERR};

static char add_philosopher();
static char delete_philosoper();

static void grap_forks(int id) {
    if(id % 2 == 0){
        call_sys_sem_wait(philosophers[id].left_fork);
        call_sys_sem_wait(philosophers[id].right_fork);
    }else{  
        call_sys_sem_wait(philosophers[id].right_fork);
        call_sys_sem_wait(philosophers[id].left_fork);
    }
}

static void check_and_grab_forks(int id) {
    philosophers[id].state = HUNGRY;
    int left = (id + amount_of_philosophers - 1) % amount_of_philosophers;
    int right = (id + 1) % amount_of_philosophers;

    if (philosophers[left].state != EATING && philosophers[right].state != EATING) {
        philosophers[id].state = EATING;
    }
}

static void release_forks(int id) {
    int left = (id + amount_of_philosophers - 1) % amount_of_philosophers;
    int right = (id + 1) % amount_of_philosophers;

    philosophers[id].state = THINKING;

    check_and_grab_forks(left);
    check_and_grab_forks(right);
}


void philosopher_process(int argc, char **argv) {
    int id = my_atoi(argv[0]);

    while (1) {
        call_sys_sem_wait(mutex);
        philosophers[id].state = THINKING;
        call_sys_sem_post(mutex);
        print_table_state();
        call_sys_sleep(2 + (id % 3)); 

        call_sys_sem_wait(mutex);
        check_and_grab_forks(id);
        call_sys_sem_post(mutex);

        if (philosophers[id].state == EATING) {
            call_sys_sleep(2 + (id % 3));
        }

        call_sys_sem_wait(mutex);
        release_forks(id);
        print_table_state();
        call_sys_sem_post(mutex);
    }
}


void print_table_state() {
    for (int i = 0; i < amount_of_philosophers; i++) {
        if (philosophers[i].state == EATING) { 
            printf("%c",'E');
        } else {
            printf("%c",'.');
        }
        printf(" ");
    }
    printf("\n");
}

static char add_philosopher(){
    call_sys_sem_wait(mutex);
    if(amount_of_philosophers >= MAX_PHILOSOPHERS){
        call_sys_error("Error: max philosophers reached\n", STDERR);
        call_sys_sem_post(mutex);
        return -1;
    }

    int i = amount_of_philosophers;
    philosophers[i].state = THINKING;

    strcpy(philosophers[i].left_fork, my_itoa(i));
    call_sys_sem_open(philosophers[i].left_fork, 1);

    if (i > 0) {
        strcpy(philosophers[i].right_fork, my_itoa((i + 1) % MAX_PHILOSOPHERS));
        call_sys_sem_open(philosophers[i].right_fork, 1);
        strcpy(philosophers[i - 1].right_fork, philosophers[i].left_fork);
    } else {
        strcpy(philosophers[i].right_fork, my_itoa(1));
    }

    char *args[1] = {my_itoa(i)};
    philosophers[i].pid = call_sys_create_process("Philosopher", 4, args, 1, &philosopher_process, philosopher_fds);

    amount_of_philosophers++;
    call_sys_sem_post(mutex);

    return 0;
}

static char delete_philosoper(){
    call_sys_sem_wait(mutex);
    int i = amount_of_philosophers-1;
    call_sys_sem_close(philosophers[i].left_fork);
    call_sys_sem_close(philosophers[i].right_fork);
    call_sys_kill_process(philosophers[i].pid);
    call_sys_wait_children(philosophers[i].pid);
    amount_of_philosophers--;
    call_sys_sem_post(mutex);
    return 0;
}


void table_manager() {
    char input;
    while ((input = get_char()) != QUIT) {
        switch(input) {
            case ADD:
                add_philosopher();
                printf("Added philosopher\n");
                break;
            case REMOVE:
                delete_philosoper();
                printf("Removed philosopher\n");
                break;
            default:
                break;
        }
    }
    

    int actual_philo = amount_of_philosophers;
    for(int i=0; i<actual_philo; i++){
        delete_philosoper();
    }
    call_sys_sem_close(mutex);
    call_sys_enter();
}

static void header(){
    printf("Press 'a' to ADD a philosopher\n");
    printf("Press 'r' to REMOVE a philosopher\n");
    printf("Press 'q' to QUIT\n");
    printf("----------------------------------\n");
}

int initialize_philosophers() {
    call_sys_sem_open(mutex, 1);

    int parent_pid = call_sys_create_process("Table Manager", 4, NULL, 0, &table_manager, fds);
    if(parent_pid == -1){
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
    call_sys_wait_children(parent_pid); // Blockeo shell hasta que termine el proceso
    return parent_pid;
}
