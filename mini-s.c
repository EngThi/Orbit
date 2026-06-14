#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 256
#define MAX_ARGS 20

void salvar_historico(char *input) {
  FILE *f = fopen("history.txt", "a");
  if (f != NULL) {
    fprintf(f, "%s\n", input);
    fclose(f);
  }
}

void limpar_historico() {
  FILE *f = fopen("history.txt", "w");
  if (f != NULL) {
    fclose(f);
  }
}

int parse_input(char *input, char **args) {
  int i = 0;

  args[i] = strtok(input, " ");
  while (args[i] != NULL && i < MAX_ARGS - 1) {
    i++;
    args[i] = strtok(NULL, " ");
  }

  return i;
}

void cmd_echo(char **args) {
  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
}

void cmd_sum(char **args) {
  int total = 0;

  for (int i = 1; args[i] != NULL; i++) {
    total += atoi(args[i]);
  }

  printf("Resultado: %d\n", total);
}

void cmd_run(char **args) {
  if (args[1] == NULL) {
    printf("Uso: run <comando>\n");
    return;
  }

  system(args[1]);
}

void cmd_cd(char **args) {
  if (args[1] == NULL) {
    printf("Uso: cd <diretorio>\n");
    return;
  }

  if (chdir(args[1]) != 0) {
    printf("Erro ao mudar diretório\n");
  }
}

void cmd_history(char **args) {
  FILE *f = fopen("history.txt", "r");
  if (f == NULL) {
    printf("Sem histórico\n");
    return;
  }

  char linha[256];
  int count = 1;

  while (fgets(linha, sizeof(linha), f)) {
    printf("%d: %s", count, linha);
    count++;
  }

  fclose(f);
}

void cmd_clear_history(char **args) {
  limpar_historico();
  printf("Histórico limpo\n");
}

void cmd_repeat(char **args) {
  if (args[1] == NULL) {
    printf("Uso: repeat <numero>\n");
    return;
  }

  int alvo = atoi(args[1]);

  FILE *f = fopen("history.txt", "r");
  if (f == NULL) {
    printf("Erro ao abrir histórico\n");
    return;
  }

  char linha[256];
  int count = 1;
  int encontrado = 0;

  while (fgets(linha, sizeof(linha), f)) {
    if (count == alvo) {
      linha[strcspn(linha, "\n")] = '\0'; // remove \n
      printf("Executando: %s\n", linha);
      system(linha);
      encontrado = 1;
      break;
    }
    count++;
  }

  if (!encontrado) {
    printf("Linha não encontrada\n");
  }

  fclose(f);
}

typedef struct {
  char *name;
  void (*func)(char **args);
} Command;

Command commands[] = {
    {"echo", cmd_echo},       {"sum", cmd_sum},
    {"run", cmd_run},         {"cd", cmd_cd},
    {"history", cmd_history}, {"clear_history", cmd_clear_history},
    {"repeat", cmd_repeat}};

int total_commands = sizeof(commands) / sizeof(Command);

// ===================== HELP =====================

void cmd_help(char **args) {
  printf("Comandos disponíveis:\n");

  for (int i = 0; i < total_commands; i++) {
    printf("- %s\n", commands[i].name);
  }

  printf("- exit\n");
}

int main() {
  char input[MAX_INPUT];
  char *args[MAX_ARGS];

  while (1) {
    printf("> ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }

    char original[MAX_INPUT];
    strcpy(original, input);

    input[strcspn(input, "\n")] = '\0';
    original[strcspn(original, "\n")] = '\0';

    if (strlen(original) > 0) {
      salvar_historico(original);
    }

    parse_input(input, args);

    if (args[0] == NULL)
      continue;

    if (strcmp(args[0], "exit") == 0) {
      printf("Saindo...\n");
      break;
    }

    if (strcmp(args[0], "help") == 0) {
      cmd_help(args);
      continue;
    }

    // dispatcher
    int encontrado = 0;

    for (int i = 0; i < total_commands; i++) {
      if (strcmp(args[0], commands[i].name) == 0) {
        commands[i].func(args);
        encontrado = 1;
        break;
      }
    }

    if (!encontrado) {
      printf("Comando desconhecido\n");
    }
  }

  return 0;
}