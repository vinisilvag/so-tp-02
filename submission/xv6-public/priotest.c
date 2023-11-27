#include "types.h"
#include "stat.h"
#include "user.h"
#include <stddef.h>

int main(int argc, char *argv[])
{
  int proc_count = 20;

  for (int i = 0; i < proc_count; i++) {
    int pid = fork();

    if (pid == 0) {
      for (int j = 0; j < 100; j++)
        for (int k = 0; k < 1000000; k++)
          asm("nop");

      exit();
    } else {
      // Pai seta a prioridade do filho
      set_prio((pid % 3) + 1, pid);
    }
  }

  printf(1, "Processos finalizados:\n");
  for (int i = 0; i < proc_count; i++) {
    int priority, retime, rutime;
    int pid = wait3(&priority, &retime, &rutime);

    printf(1, "PID: %d - Prioridade ao finalizar: %d - Tempo na fila de READY: %d - Tempo na fila de RUNNING: %d\n", pid, priority, retime, rutime);
  }

  exit();
}