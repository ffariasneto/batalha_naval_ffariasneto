#include <stdio.h>

#define TAM_TABULEIRO 10
#define TAM_NAVIO 3
#define AGUA 0
#define NAVIO 3

// *** NOVO BLOCO NIVEL AVANCADO: CONSTANTES DE HABILIDADE ***
#define TAM_HAB 5        // tamanho das matrizes de habilidade (5x5)
#define HABILIDADE 5     // valor usado no tabuleiro para marcar área de habilidade

/* 
 * Inicializa o tabuleiro com água (0 em todas as posições).
 */
void inicializarTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    for (int linha = 0; linha < TAM_TABULEIRO; linha++) {
        for (int coluna = 0; coluna < TAM_TABULEIRO; coluna++) {
            tabuleiro[linha][coluna] = AGUA;
        }
    }
}

/*
 * Valida se um navio horizontal cabe no tabuleiro a partir da posição (linha, coluna)
 * e se não sobrepõe outro navio.
 */
int validarPosicaoHorizontal(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                             int linha, int coluna) {
    // Verifica se a linha está dentro do tabuleiro
    if (linha < 0 || linha >= TAM_TABULEIRO) {
        return 0; // inválido
    }

    // Verifica se a coluna inicial + tamanho do navio cabem no tabuleiro
    if (coluna < 0 || (coluna + TAM_NAVIO) > TAM_TABULEIRO) {
        return 0; // inválido
    }

    // Verifica se as posições já estão ocupadas
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (tabuleiro[linha][coluna + i] != AGUA) {
            return 0; // já tem navio ali, sobreposição
        }
    }

    return 1; // posição válida
}

/*
 * Valida se um navio vertical cabe no tabuleiro a partir da posição (linha, coluna)
 * e se não sobrepõe outro navio.
 */
int validarPosicaoVertical(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                           int linha, int coluna) {
    // Verifica se a coluna está dentro do tabuleiro
    if (coluna < 0 || coluna >= TAM_TABULEIRO) {
        return 0; // inválido
    }

    // Verifica se a linha inicial + tamanho do navio cabem no tabuleiro
    if (linha < 0 || (linha + TAM_NAVIO) > TAM_TABULEIRO) {
        return 0; // inválido
    }

    // Verifica se as posições já estão ocupadas
    for (int i = 0; i < TAM_NAVIO; i++) {
        if (tabuleiro[linha + i][coluna] != AGUA) {
            return 0; // já tem navio ali, sobreposição
        }
    }

    return 1; // posição válida
}

/*
 * Posiciona um navio horizontalmente, assumindo que já foi validado.
 * As posições ocupadas recebem o valor NAVIO (3).
 */
void posicionarNavioHorizontal(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                               int navio[TAM_NAVIO],
                               int linha, int coluna) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha][coluna + i] = navio[i];
    }
}

/*
 * Posiciona um navio verticalmente, assumindo que já foi validado.
 * As posições ocupadas recebem o valor NAVIO (3).
 */
void posicionarNavioVertical(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                             int navio[TAM_NAVIO],
                             int linha, int coluna) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        tabuleiro[linha + i][coluna] = navio[i];
    }
}

/* ********************************************************************
 * *** NOVO BLOCO NIVEL AVENTUREIRO: FUNÇÕES PARA NAVIOS DIAGONAIS ***
 * ********************************************************************
 *
 * direcao =  1 -> diagonal descendo para a direita (linha+i, coluna+i)
 * direcao = -1 -> diagonal descendo para a esquerda (linha+i, coluna-i)
 */

/*
 * Valida se um navio diagonal cabe no tabuleiro a partir da posição
 * (linha, coluna) e se não há sobreposição com outros navios.
 */
int validarPosicaoDiagonal(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                           int linha, int coluna, int direcao) {
    // Verifica se direcao é válida (1 ou -1)
    if (direcao != 1 && direcao != -1) {
        return 0;
    }

    // Verificação de limites dependendo da direção
    if (linha < 0 || linha + TAM_NAVIO > TAM_TABULEIRO) {
        return 0;
    }

    if (direcao == 1) {
        // Diagonal principal: coluna aumenta
        if (coluna < 0 || coluna + TAM_NAVIO > TAM_TABULEIRO) {
            return 0;
        }
    } else {
        // Diagonal secundária: coluna diminui
        if (coluna >= TAM_TABULEIRO || coluna - (TAM_NAVIO - 1) < 0) {
            return 0;
        }
    }

    // Verifica se as posições já estão ocupadas
    for (int i = 0; i < TAM_NAVIO; i++) {
        int linhaAtual = linha + i;
        int colunaAtual = coluna + (direcao * i);

        if (tabuleiro[linhaAtual][colunaAtual] != AGUA) {
            return 0; // sobreposição
        }
    }

    return 1; // posição válida
}

/*
 * Posiciona um navio na diagonal, assumindo que já foi validado.
 */
void posicionarNavioDiagonal(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                             int navio[TAM_NAVIO],
                             int linha, int coluna, int direcao) {
    for (int i = 0; i < TAM_NAVIO; i++) {
        int linhaAtual = linha + i;
        int colunaAtual = coluna + (direcao * i);
        tabuleiro[linhaAtual][colunaAtual] = navio[i];
    }
}

/* **********************************************************************
 * *** NOVO BLOCO NIVEL AVANCADO: MATRIZES E LOGICA DAS HABILIDADES  ***
 * **********************************************************************
 *
 * Aqui criamos e preenchemos as matrizes de habilidades:
 *  - Cone (apontando para baixo, tipo triângulo)
 *  - Cruz
 *  - Octaedro visto de frente (losango)
 *
 * Cada matriz é TAM_HAB x TAM_HAB e usa:
 *  0 -> fora da área da habilidade
 *  1 -> dentro da área da habilidade
 *
 * Depois, sobrepomos essa matriz ao tabuleiro, marcando com valor 5.
 */

/*
 * Função auxiliar para valor absoluto de um inteiro (sem usar stdlib).
 */
int valorAbsoluto(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

/*
 * Constrói uma matriz em forma de CONE apontando para baixo.
 * Usaremos as 3 primeiras linhas da matriz 5x5:
 *
 *   linha 0:   0 0 1 0 0
 *   linha 1:   0 1 1 1 0
 *   linha 2:   1 1 1 1 1
 *   (restante 0)
 */
void construirHabilidadeCone(int hab[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2; // para TAM_HAB=5, centro = 2

    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            // Por padrão, sem efeito
            hab[i][j] = 0;

            // Usar apenas as 3 primeiras linhas do cone
            if (i <= 2) {
                int distColuna = valorAbsoluto(j - centro);
                // Quanto mais embaixo, maior a largura (|j-centro| <= i)
                if (distColuna <= i) {
                    hab[i][j] = 1;
                }
            }
        }
    }
}

/*
 * Constrói uma matriz em forma de CRUZ.
 * Centro em (2,2). Fica assim:
 *
 *   0 0 1 0 0
 *   0 0 1 0 0
 *   1 1 1 1 1
 *   0 0 1 0 0
 *   0 0 1 0 0
 */
void construirHabilidadeCruz(int hab[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;

    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            if (i == centro || j == centro) {
                hab[i][j] = 1;
            } else {
                hab[i][j] = 0;
            }
        }
    }
}

/*
 * Constrói uma matriz em forma de OCTAEDRO (losango),
 * usando distância de Manhattan:
 *
 * |i-centro| + |j-centro| <= centro
 *
 * Para TAM_HAB=5, centro=2:
 *
 *   0 0 1 0 0
 *   0 1 1 1 0
 *   1 1 1 1 1
 *   0 1 1 1 0
 *   0 0 1 0 0
 */
void construirHabilidadeOctaedro(int hab[TAM_HAB][TAM_HAB]) {
    int centro = TAM_HAB / 2;

    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            int distLinha = valorAbsoluto(i - centro);
            int distColuna = valorAbsoluto(j - centro);
            if (distLinha + distColuna <= centro) {
                hab[i][j] = 1;
            } else {
                hab[i][j] = 0;
            }
        }
    }
}

/*
 * Sobrepõe uma matriz de habilidade ao tabuleiro.
 *
 * - origemLinha e origemColuna são o "centro" da habilidade no tabuleiro.
 * - A matriz TAM_HAB x TAM_HAB é centrada nesse ponto.
 * - Onde a matriz tiver 1, marcamos HABILIDADE (5) no tabuleiro,
 *   desde que esteja dentro dos limites e não sobrescreva navio (3).
 */
void aplicarHabilidadeNoTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                                  int hab[TAM_HAB][TAM_HAB],
                                  int origemLinha, int origemColuna) {
    int offset = TAM_HAB / 2; // para centralizar a matriz

    for (int i = 0; i < TAM_HAB; i++) {
        for (int j = 0; j < TAM_HAB; j++) {
            if (hab[i][j] == 1) {
                int linhaTab = origemLinha + (i - offset);
                int colunaTab = origemColuna + (j - offset);

                // Verifica se está dentro do tabuleiro
                if (linhaTab >= 0 && linhaTab < TAM_TABULEIRO &&
                    colunaTab >= 0 && colunaTab < TAM_TABULEIRO) {

                    // Não sobrescrever navios, apenas água ou outra habilidade
                    if (tabuleiro[linhaTab][colunaTab] != NAVIO) {
                        tabuleiro[linhaTab][colunaTab] = HABILIDADE;
                    }
                }
            }
        }
    }
}

/*
 * Exibe o tabuleiro no console.
 * 0 = água, 3 = navio, 5 = área de habilidade.
 */
void exibirTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    printf("   ");
    // Cabeçalho com números das colunas
    for (int coluna = 0; coluna < TAM_TABULEIRO; coluna++) {
        printf("%2d ", coluna);
    }
    printf("\n");

    for (int linha = 0; linha < TAM_TABULEIRO; linha++) {
        printf("%2d ", linha); // índice da linha
        for (int coluna = 0; coluna < TAM_TABULEIRO; coluna++) {
            printf("%2d ", tabuleiro[linha][coluna]);
        }
        printf("\n");
    }
}

int main() {
    // Matriz representando o tabuleiro 10x10
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];

    // Vetores representando os navios (tamanho fixo 3)
    int navioHorizontal[TAM_NAVIO] = {NAVIO, NAVIO, NAVIO};
    int navioVertical[TAM_NAVIO]   = {NAVIO, NAVIO, NAVIO};

    /* ************************************************************
     * *** NOVO BLOCO NIVEL AVENTUREIRO: NAVIOS DIAGONAIS NO MAIN *
     * ************************************************************
     *
     * Dois navios adicionais, que serão posicionados na diagonal.
     */
    int navioDiagonal1[TAM_NAVIO]  = {NAVIO, NAVIO, NAVIO};
    int navioDiagonal2[TAM_NAVIO]  = {NAVIO, NAVIO, NAVIO};

    // *** NOVO BLOCO NIVEL AVANCADO: MATRIZES DE HABILIDADES ***
    int habilidadeCone[TAM_HAB][TAM_HAB];
    int habilidadeCruz[TAM_HAB][TAM_HAB];
    int habilidadeOctaedro[TAM_HAB][TAM_HAB];

    // Inicializa o tabuleiro com água (0)
    inicializarTabuleiro(tabuleiro);

    // Coordenadas iniciais definidas diretamente no código
    // Navio horizontal começando na linha 2, coluna 1
    int linhaNavioH = 2;
    int colunaNavioH = 1;

    // Navio vertical começando na linha 5, coluna 7
    int linhaNavioV = 5;
    int colunaNavioV = 7;

    // Valida e posiciona o navio horizontal
    if (validarPosicaoHorizontal(tabuleiro, linhaNavioH, colunaNavioH)) {
        posicionarNavioHorizontal(tabuleiro, navioHorizontal,
                                  linhaNavioH, colunaNavioH);
    } else {
        printf("Erro: nao foi possivel posicionar o navio horizontal em (%d, %d).\n",
               linhaNavioH, colunaNavioH);
        return 1;
    }

    // Valida e posiciona o navio vertical
    if (validarPosicaoVertical(tabuleiro, linhaNavioV, colunaNavioV)) {
        posicionarNavioVertical(tabuleiro, navioVertical,
                                linhaNavioV, colunaNavioV);
    } else {
        printf("Erro: nao foi possivel posicionar o navio vertical em (%d, %d).\n",
               linhaNavioV, colunaNavioV);
        return 1;
    }

    /* 
     * Escolha das coordenadas para os navios diagonais.
     * Cuidei para não sobrepor com os navios já posicionados.
     *
     * navioDiagonal1 -> diagonal descendo para a direita (direcao = 1)
     * navioDiagonal2 -> diagonal descendo para a esquerda (direcao = -1)
     */

    // navioDiagonal1 começando em (0, 7): ocupa (0,7), (1,8), (2,9)
    int linhaNavioD1 = 0;
    int colunaNavioD1 = 7;
    int direcaoD1 = 1;  // principal

    // navioDiagonal2 começando em (4, 2): ocupa (4,2), (5,1), (6,0)
    int linhaNavioD2 = 4;
    int colunaNavioD2 = 2;
    int direcaoD2 = -1; // secundária

    // Valida e posiciona o primeiro navio diagonal
    if (validarPosicaoDiagonal(tabuleiro, linhaNavioD1, colunaNavioD1, direcaoD1)) {
        posicionarNavioDiagonal(tabuleiro, navioDiagonal1,
                                linhaNavioD1, colunaNavioD1, direcaoD1);
    } else {
        printf("Erro: nao foi possivel posicionar o navio diagonal 1 em (%d, %d).\n",
               linhaNavioD1, colunaNavioD1);
        return 1;
    }

    // Valida e posiciona o segundo navio diagonal
    if (validarPosicaoDiagonal(tabuleiro, linhaNavioD2, colunaNavioD2, direcaoD2)) {
        posicionarNavioDiagonal(tabuleiro, navioDiagonal2,
                                linhaNavioD2, colunaNavioD2, direcaoD2);
    } else {
        printf("Erro: nao foi possivel posicionar o navio diagonal 2 em (%d, %d).\n",
               linhaNavioD2, colunaNavioD2);
        return 1;
    }

    /* ************************************************************
     * *** NOVO BLOCO NIVEL AVANCADO: CONSTRUCAO DAS HABILIDADES ***
     * ************************************************************
     *
     * Preenche as matrizes de habilidade Cone, Cruz e Octaedro
     * usando loops e condicionais.
     */
    construirHabilidadeCone(habilidadeCone);
    construirHabilidadeCruz(habilidadeCruz);
    construirHabilidadeOctaedro(habilidadeOctaedro);

    /* **************************************************************
     * *** NOVO BLOCO NIVEL AVANCADO: APLICACAO DAS HABILIDADES   ***
     * **************************************************************
     *
     * Definimos pontos de origem no tabuleiro para cada habilidade.
     * A matriz 5x5 é centralizada nesses pontos.
     */

    // Ponto de origem do CONE
    int origemConeLinha = 2;
    int origemConeColuna = 2;

    // Ponto de origem da CRUZ
    int origemCruzLinha = 5;
    int origemCruzColuna = 5;

    // Ponto de origem do OCTAEDRO
    int origemOctaedroLinha = 7;
    int origemOctaedroColuna = 3;

    // Aplica as habilidades ao tabuleiro
    aplicarHabilidadeNoTabuleiro(tabuleiro, habilidadeCone,
                                 origemConeLinha, origemConeColuna);

    aplicarHabilidadeNoTabuleiro(tabuleiro, habilidadeCruz,
                                 origemCruzLinha, origemCruzColuna);

    aplicarHabilidadeNoTabuleiro(tabuleiro, habilidadeOctaedro,
                                 origemOctaedroLinha, origemOctaedroColuna);

    // Exibe o tabuleiro com os navios e as áreas de habilidade
    printf("\nTabuleiro de Batalha Naval:\n");
    printf("0 = agua, 3 = navio, 5 = area de habilidade\n\n");
    exibirTabuleiro(tabuleiro);

    return 0;
}
