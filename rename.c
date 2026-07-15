#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define PAGES_DIR "pages"

/*
 * Descobre automaticamente o número da última página existente,
 * olhando os arquivos "N.png" dentro da pasta pages/.
 */
int detect_total_pages() {
    DIR *dir = opendir(PAGES_DIR);
    if (dir == NULL) {
        printf("Erro: nao encontrei a pasta '%s/'. Rode este programa a partir da raiz do projeto.\n", PAGES_DIR);
        exit(1);
    }

    struct dirent *entry;
    int max_page = 0;

    while ((entry = readdir(dir)) != NULL) {
        int n;
        // Considera apenas arquivos no formato "N.png" (ex: 1.png, 23.png)
        if (sscanf(entry->d_name, "%d.png", &n) == 1) {
            if (n > max_page)
                max_page = n;
        }
    }

    closedir(dir);

    if (max_page == 0) {
        printf("Erro: nao encontrei nenhum arquivo 'N.png' dentro de '%s/'.\n", PAGES_DIR);
        exit(1);
    }

    return max_page;
}

int main(int argc, char *argv[]) {
    int total_pages;
    int insert_pos;

    // Descobre automaticamente quantas paginas existem
    total_pages = detect_total_pages();
    printf("Paginas encontradas em '%s/': %d\n", PAGES_DIR, total_pages);

    // A posicao de insercao pode vir por argumento (./rename 15)
    // ou ser perguntada interativamente, se nao for informada.
    if (argc >= 2) {
        insert_pos = atoi(argv[1]);
    } else {
        printf("Em qual posicao a nova pagina vai entrar? (1 a %d): ", total_pages + 1);
        if (scanf("%d", &insert_pos) != 1) {
            printf("Erro: entrada invalida.\n");
            return 1;
        }
    }

    // Validacao da posicao informada
    if (insert_pos < 1 || insert_pos > total_pages + 1) {
        printf("Erro: posicao invalida. Deve estar entre 1 e %d.\n", total_pages + 1);
        return 1;
    }

    char old_name[256];
    char new_name[256];

    // O loop precisa rodar de tras para frente para nao sobrescrever os arquivos
    for (int i = total_pages; i >= insert_pos; i--) {
        snprintf(old_name, sizeof(old_name), "%s/%d.png", PAGES_DIR, i);
        snprintf(new_name, sizeof(new_name), "%s/%d.png", PAGES_DIR, i + 1);

        if (rename(old_name, new_name) == 0) {
            printf("Sucesso: %d.png -> %d.png\n", i, i + 1);
        } else {
            printf("Erro ao renomear o arquivo %d.png. Verifique se ele existe e se voce tem permissao.\n", i);
        }
    }

    printf("\nPronto! O espaco foi aberto. Agora voce pode salvar sua nova imagem como %s/%d.png\n", PAGES_DIR, insert_pos);

    // Pausa no final para quem der dois cliques no .exe conseguir ler o resultado
    // antes da janela fechar sozinha.
    printf("\nPressione ENTER para sair...");
    // Limpa o buffer de entrada (sobra do scanf, se foi usado) antes de esperar o Enter final
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();

    return 0;
}
