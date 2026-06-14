#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void salvar_historico(char *input) {
  FILE *file = fopen("history.txt", "a");

  if (file == NULL) {
    printf("Erro ao abrir arquivo\n");
    return;
  }

  fprintf(file, "%s\n", input);
  fclose(file);
}

void limpar_historico() {
  FILE *file = fopen("history.txt", "w");

  if (file == NULL) {
    printf("Erro ao limpar histórico\n");
    return;
  }

  fclose(file);
  printf("Histórico limpo!\n");
}

int main() {
  char input[100];
  char *comandos[] = {"help",          "echo", "sum", "history",
                      "clear_history", "run",  "cd",  "repeat"};

  int total_comandos = 8;

  while (1) {
    printf("> ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\r\n")] = 0;

    salvar_historico(input);

    char *cmd = strtok(input, " ");

    if (cmd == NULL)
      continue;

    if (strcmp(cmd, "exit") == 0) {
      printf("Saindo...\n");
      break;
    } else if (strcmp(cmd, "cd") == 0) {
      char *path = strtok(NULL, " ");

      if (path == NULL) {
        printf("Use: cd <diretorio>\n");
      } else {
        if (chdir(path) != 0) {
          printf("Erro ao mudar diretório\n");
        }
      }
    } else if (strcmp(cmd, "history") == 0) {
      FILE *file = fopen("history.txt", "r");

      if (file == NULL) {
        printf("Nenhum histórico encontrado\n");
        continue;
      }

      char linha[100];
      int contador = 1;

      while (fgets(linha, sizeof(linha), file)) {
        printf("%d: %s", contador, linha);
        contador++;
      }

      fclose(file);
    }

    else if (strcmp(cmd, "clear_history") == 0) {
      limpar_historico();
    }

    else if (strcmp(cmd, "run") == 0) {
      char comando[100] = "";

      char *token = strtok(NULL, " ");

      if (token == NULL) {
        printf("Use: run <comando>\n");
        continue;
      }

      while (token != NULL) {
        strcat(comando, token);
        strcat(comando, " ");
        token = strtok(NULL, " ");
      }

      printf("Executando: %s\n", comando);
      system(comando);
    }

    else if (strcmp(cmd, "repeat") == 0) {
      char *arg = strtok(NULL, " ");

      if (arg == NULL) {
        printf("Uso: repeat <numero>\n");
        continue;
      }

      int num = atoi(arg);
      FILE *file = fopen("history.txt", "r");

      if (file == NULL) {
        printf("Nenhum histórico encontrado\n");
        continue;
      }

      char linha_arq[100];
      int contador = 0;
      char comando_salvo[100] = "";
      int encontrado = 0;

      while (fgets(linha_arq, sizeof(linha_arq), file)) {
        contador++;
        if (contador == num) {
          strcpy(comando_salvo, linha_arq);
          encontrado = 1;
          break;
        }
      }

      fclose(file);

      if (encontrado) {
        comando_salvo[strcspn(comando_salvo, "\r\n")] = 0;
        printf("Executando: %s\n", comando_salvo);
        system(comando_salvo);
      } else {
        printf("Linha %d não encontrada no histórico\n", num);
      }
    }

    // help
    else if (strcmp(cmd, "help") == 0) {
      printf("Comandos disponíveis:\n");

      for (int i = 0; i < total_comandos; i++) {
        printf("- %s\n", comandos[i]);
      }
    }

    // echo
    else if (strcmp(cmd, "echo") == 0) {
      char *token = strtok(NULL, " ");
      while (token != NULL) {
        printf("%s ", token);
        token = strtok(NULL, " ");
      }
      printf("\n");
    }

    // sum
    else if (strcmp(cmd, "sum") == 0) {
      char *token = strtok(NULL, " ");
      int total = 0;

      while (token != NULL) {
        total += atoi(token);
        token = strtok(NULL, " ");
      }

      printf("Resultado: %d\n", total);
    }

    // comando desconhecido
    else {
      printf("Comando desconhecido\n");
    }
  }

  return 0;
}