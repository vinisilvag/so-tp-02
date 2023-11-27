#include "types.h"
#include "stat.h"
#include "user.h"
#include <stddef.h>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf(1, "Um numero natural n deve ser provido.\n");
    exit();
  }

  if (argc > 2) {
    printf(1, "Muitos argumentos foram providos.\n");
    exit();
  }

  int n = atoi(argv[1]);

  if (n <= 0) {
    printf(1, "Valor de n invalido (n deve ser > 0).\n");
    exit();
  }

  int number_of_process = 3 * n;

  int proc_type_number[3], re_mean[3], ru_mean[3], s_mean[3], turn_around_mean[3];

  for (int i = 0; i < 3; i++) {
    proc_type_number[i] = 0;
    re_mean[i] = 0;
    ru_mean[i] = 0;
    s_mean[i] = 0;
    turn_around_mean[i] = 0;
  }

  for (int i = 0; i < number_of_process; i++) {
    int pid = fork();

    if (pid == 0) {
      int proc_type = getpid() % 3;

      if (proc_type == 0) {
        // CPU-Bound

        // nop usado pois eh o mais proximo de executar os loops
        // "sem fazer nada" mas ainda executar alguma instrucao de maquina
        for (int j = 0; j < 100; j++)
          for (int k = 0; k < 1000000; k++)
            asm("nop");
      } else if (proc_type == 1) {
        // S-CPU

        // nop usado pois eh o mais proximo de executar os loops
        // "sem fazer nada" mas ainda executar alguma instrucao de maquina
        for (int j = 0; j < 20; j++) {
          for (int k = 0; k < 1000000; k++)
            asm("nop");
          yield2();
        }
      } else {
        // IO-Bound

        for (int j = 0; j < 100; j++)
          sleep(1);
      }

      exit();
    } else {
      proc_type_number[pid % 3]++;
    }
  }

  char* type[3] = { "CPU-Bound", "S-Bound", "IO-Bound" };

  for (int i = 0; i < number_of_process; i++) {
    int retime, rutime, stime;
    int pid = wait2(&retime, &rutime, &stime);

    re_mean[pid % 3] += retime;
    ru_mean[pid % 3] += rutime;
    s_mean[pid % 3] += stime;
    turn_around_mean[pid % 3] += (retime + rutime + stime);

    printf(1, "PID: %d - Tipo de processo: %s - Tempo de espera: %d - Tempo executando: %d - Tempo de IO: %d\n", pid, type[pid % 3], retime, rutime, stime);
  }

  printf(1, "\n");

  for (int i = 0; i < 3; i++) {
    printf(1, "Processos: %s\n", type[i]);
    printf(1, "Total de processos: %d\n", proc_type_number[i]);
    printf(1, "Tempo medio no estado RUNNING: %d\n", ru_mean[i] / proc_type_number[i]);
    printf(1, "Tempo medio no estado READY: %d\n", re_mean[i] / proc_type_number[i]);
    printf(1, "Tempo medio no estado SLEEPING: %d\n", s_mean[i] / proc_type_number[i]);
    printf(1, "Tempo medio de TURNAROUND: %d\n\n", turn_around_mean[i] / proc_type_number[i]);
  }

  exit();
}