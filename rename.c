#include <stdio.h>
#include <stdlib.h>

int main() {
    int total_pages = 70; // Altere para o seu total de páginas atual
    int insert_pos = 15;  // Altere para a posição onde a nova página vai entrar

    char old_name[100];
    char new_name[100];

    // O loop precisa rodar de trás para frente para não sobrescrever os arquivos
    for (int i = total_pages; i >= insert_pos; i--) {
        sprintf(old_name, "pages/%d.png", i);
        sprintf(new_name, "pages/%d.png", i + 1);
        
        if (rename(old_name, new_name) == 0) {
            printf("Sucesso: %d.png -> %d.png\n", i, i + 1);
        } else {
            printf("Erro ao renomear o arquivo %d.png. Verifique se ele existe.\n", i);
        }
    }
    
    printf("\nPronto! O espaco foi aberto. Agora voce pode salvar sua nova imagem como pages/%d.png\n", insert_pos);
    return 0;
}