
//sgbd em homenagem ao julio na gaita e a bicharada no vocal cantando um rock rural cocorico!

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//Estruturas ultilizadas

    struct Cliente
{
    char Nome[100];
    char CPF[15];
    char Cell[20]; 
};

    struct Veiculo
{
    char Placa[8];
    char modelo[100];
    int ano;
    char propCpf[15];
};

    typedef enum {
        AGUARDANDO_AVALIACAO = 0,
        EM_REPARO = 1,
        FINALIZADO = 2,
        ENTREGUE = 3,
    } Status;

    struct ordemServico
{
    char Placa[8];
    Status status;
    int id;
    char DescProblem[500];
    char DataEntrada[11];
};

//funções de suporte

bool VeriCell(const char *cell){

    if (cell == NULL) return false;

    int DigitCount = 0;

    for(int i = 0; cell[i] != '\0'; i++){

    if(cell[i] == '(' || cell[i] == ')' ||
            cell[i] == '-' || cell[i] == ' ') { 
            continue;
                            }
        
    else if (isdigit(cell[i])) {
        DigitCount++;
      }

    else {
        return false;
      }
    }
    return (DigitCount == 11 || DigitCount == 10);
};

bool VeriPlaca (const char *placa){
    if(placa == NULL) return false;
    int comprimento = strlen(placa);
    if (comprimento != 7) return false;

    bool pMS = true;
    for(int i = 0; i<7; i++){
        if(i<=2 || i==4){
            if(!isalpha(placa[i])){
            pMS = false;
            break;
        }
    }else if (!isdigit(placa[i])) {
            pMS = false;
            break;
        }
    };
    if(pMS) return true;
    bool pA = true;
    for(int i = 0; i<7; i++){
        if(i<=2){
            if(!isalpha(placa[i])){
            pA = false;
            break;
        }
    }else if(!isdigit(placa[i])) {
            pA = false;
            break;
        }
    };
    return pA;
};

bool VeriCpf(const char *cpf){
    if (cpf == NULL) return false;
    int digitos = 0;
    int comprimento = strlen(cpf);


    if (comprimento > 15) { // 11 dígitos + 3 formatações
        return false;
    }

    for(int i = 0; cpf[i] != '\0'; i++){
      if (isalpha(cpf[i])) {
        return false;
      }
      if (isdigit(cpf[i])) {
        digitos++;
        }
    }
    return digitos==11;
};

bool VeriDate(const char *date) {
    if (strlen(date) != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;
    
    int day, month, year;
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) return false;
    
    if (year < 1900 || year > 2025) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        if (day > 29) return false;
        if (day == 29 && !((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) return false;
    }
    
    return true;
};

void cleanCpf(const char *cpf, char *cleanCpf) {
    int j = 0;
    for (int i = 0; cpf[i] != '\0' && j < 11; i++) {
        if (isdigit(cpf[i])) {      
            cleanCpf[j] = cpf[i];   
            j++;                    
        }
    }
    cleanCpf[j] = '\0';             
};

void formataCpf(const char *cpf, char *cpfformatado){
    char Cpflimpo[12] = {0};
    cleanCpf(cpf, Cpflimpo);
    if (strlen(Cpflimpo) == 11) {
        sprintf(cpfformatado, "%.3s.%.3s.%.3s-%.2s", 
                Cpflimpo, Cpflimpo + 3, Cpflimpo + 6, Cpflimpo + 9);
    } else {
        strcpy(cpfformatado, cpf);
    }
};

struct Cliente* searchPropByCpf(const char *searchCpf){
    FILE *arqv = fopen("clientes.csv", "r");
    if (arqv == NULL) {
        printf("Erro em searchProp: arqv not found \n");
        return NULL;
    }

    char line[200];
    char searchCpfClean[12];
    char arqvCpfClean[12];

    cleanCpf(searchCpf, searchCpfClean); 

    if (fgets(line, sizeof(line), arqv) != NULL) {
        if (strstr(line, "Nome") != NULL || strstr(line, "CPF") != NULL) {
        } else {
            fseek(arqv, 0, SEEK_SET);
        }
    }else {
        fclose(arqv);
        return NULL;
    }

    struct Cliente *cliente = malloc(sizeof(struct Cliente));
    if (cliente == NULL) {
        fclose(arqv);
        printf("Erro em searchProp: malloc \n");
        return NULL;
    }

    while(fgets(line, sizeof(line), arqv) != NULL){
        line[strcspn(line, "\n")] = '\0';

        char *nome = strtok(line, ",");
        char *cpf = strtok(NULL, ",");
        char *cell = strtok(NULL, ",");

        if (nome == NULL || cpf == NULL || cell == NULL){
            continue;
        }

        cleanCpf(cpf, arqvCpfClean);

        if (strcmp(arqvCpfClean, searchCpfClean) ==0) {
            strcpy(cliente->Nome, nome);
            strcpy(cliente->CPF, cpf);
            strcpy(cliente->Cell, cell);

            fclose(arqv);
            return cliente;
        }
    }
    free(cliente);
    fclose(arqv);
    return NULL;
};

struct Veiculo* searchVeiByPlaca(const char *placa){
    if (placa == NULL) return NULL;
    
    FILE *arquivo = fopen("veiculos.csv", "r");
    if (arquivo == NULL) return NULL;

    char line[200];
    char placaUp[8] = {0};

    strncpy(placaUp, placa, sizeof(placaUp) - 1);
    for(int i = 0; placaUp[i]; i++) {
        placaUp[i] = toupper(placaUp[i]);
    }

    if (fgets(line, sizeof(line), arquivo) == NULL) {
        fclose(arquivo);
        return NULL;
    }

    struct Veiculo *veiculo = malloc(sizeof(struct Veiculo));
    if (veiculo == NULL) {
        fclose(arquivo);
        return NULL; 
    }

    while(fgets(line, sizeof(line), arquivo) != NULL){
        line[strcspn(line, "\n")] = '\0';

        char *placaArquivo = strtok(line, ",");
        char *modelo = strtok(NULL, ",");
        char *anoStr = strtok(NULL, ",");
        char *cpf = strtok(NULL, ",");

        if (placaArquivo != NULL){
            char placaArqUpper[8] = {0};
            strncpy(placaArqUpper, placaArquivo, sizeof(placaArqUpper) - 1);
            for(int i = 0; placaArqUpper[i]; i++) {
                placaArqUpper[i] = toupper(placaArqUpper[i]);
            }

            if (strcmp(placaArqUpper, placaUp) == 0) {
                strncpy(veiculo->Placa, placaArquivo, sizeof(veiculo->Placa) - 1);
                strncpy(veiculo->modelo, modelo ? modelo : "", sizeof(veiculo->modelo) - 1);
                veiculo->ano = anoStr ? atoi(anoStr) : 0;
                strncpy(veiculo->propCpf, cpf ? cpf : "", sizeof(veiculo->propCpf) - 1);
                
                fclose(arquivo);
                return veiculo;
            }
        }
    }

    free(veiculo);
    fclose(arquivo);
    return NULL;
};

bool PlacaExist(const char *placa) {
    struct Veiculo *veiculo = searchVeiByPlaca(placa);
    if (veiculo != NULL) {
        free(veiculo);
        return true;
    }
    return false;
};

bool CpfExist (const char *cpf){
    struct Cliente *cliente = searchPropByCpf(cpf);
    if(cliente != NULL){
        free(cliente);
        return true;
    }
    return false;
};

int getLastOrderID() {
    FILE *file = fopen("ordens_servico.csv", "r");
    if (file == NULL) {
        return 0; 
    }
    
    char line[200];
    int maxID = 0;
    
    if (fgets(line, sizeof(line), file) != NULL) {
        while (fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\n")] = '\0';
            
            char *placa = strtok(line, ",");
            char *statusStr = strtok(NULL, ",");
            char *idStr = strtok(NULL, ",");
            char *descricao = strtok(NULL, ",");
            char *data = strtok(NULL, ",");
            
            if (idStr != NULL) {
                int currentID = atoi(idStr);
                if (currentID > maxID) {
                    maxID = currentID;
                }
            }
        }
    }
    
    fclose(file);
    return maxID;
}

void RenumberOrderIDs() {
    FILE *arquivo = fopen("ordens_servico.csv", "r");
    if (arquivo == NULL) {
        return; 
    }
    

    struct ordemServico ordens[1000]; 
    int count = 0;
    char line[600];
    

    fgets(line, sizeof(line), arquivo);
    

    while (fgets(line, sizeof(line), arquivo) != NULL && count < 1000) {
        line[strcspn(line, "\n")] = '\0';
        
        char *placa = strtok(line, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");
        
        if (placa && statusStr && idStr && descricao && data) {

            if (descricao[0] == '"' && descricao[strlen(descricao)-1] == '"') {
                memmove(descricao, descricao + 1, strlen(descricao) - 2);
                descricao[strlen(descricao) - 2] = '\0';
            }
            
            strcpy(ordens[count].Placa, placa);
            ordens[count].status = atoi(statusStr);
            ordens[count].id = atoi(idStr); // Manter o ID original temporariamente
            strcpy(ordens[count].DescProblem, descricao);
            strcpy(ordens[count].DataEntrada, data);
            count++;
        }
    }
    fclose(arquivo);
    
    if (count == 0) {
        return; 
    }
    
    for (int i = 0; i < count; i++) {
        ordens[i].id = i + 1;
    }
    
    arquivo = fopen("ordens_servico.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao renumerar IDs!\n");
        return;
    }
    
    fprintf(arquivo, "Placa,Status,ID,Descricao,DataEntrada\n");
    
    for (int i = 0; i < count; i++) {
        if (strchr(ordens[i].DescProblem, ',') != NULL) {
            fprintf(arquivo, "%s,%d,%d,\"%s\",%s\n",
                    ordens[i].Placa,
                    ordens[i].status,
                    ordens[i].id,
                    ordens[i].DescProblem,
                    ordens[i].DataEntrada);
        } else {
            fprintf(arquivo, "%s,%d,%d,%s,%s\n",
                    ordens[i].Placa,
                    ordens[i].status,
                    ordens[i].id,
                    ordens[i].DescProblem,
                    ordens[i].DataEntrada);
        }
    }
    
    fclose(arquivo);
};

void ListVeiculos() {
    FILE *arquivo = fopen("veiculos.csv", "r");
    if (arquivo == NULL) {
        printf("Nenhum veiculo cadastrado!\n");
        return;
    }
    
    char line[300];
    printf("\n=== VEICULOS CADASTRADOS ===\n");
    
    if (fgets(line, sizeof(line), arquivo) != NULL) {
        printf("Placa   | Modelo               | Ano  | CPF Proprietario\n");
        printf("--------|----------------------|------|-----------------\n");
    }
    
    int count = 0;
    while (fgets(line, sizeof(line), arquivo) != NULL) {
        char lineCopy[300];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *placa = strtok(lineCopy, ",");
        char *modelo = strtok(NULL, ",");
        char *anoStr = strtok(NULL, ",");
        char *cpf = strtok(NULL, ",");
        
        if (placa && modelo && anoStr && cpf) {
            int ano = atoi(anoStr);
            

            struct Cliente *proprietario = searchPropByCpf(cpf);
            char nomeProp[100] = "Nao encontrado";
            if (proprietario != NULL) {
                strcpy(nomeProp, proprietario->Nome);
                free(proprietario);
            }
            
            printf("%-7s | %-20s | %-4d | %s (%s)\n", 
                   placa, modelo, ano, nomeProp, cpf);
            count++;
        }
    }
    
    fclose(arquivo);
    
    if (count == 0) {
        printf("Nenhum veiculo encontrado.\n");
    } else {
        printf("\nTotal: %d veiculo(s)\n", count);
    }
};

int countVeiculos(const char *cpf){
    FILE *arquivo = fopen("veiculos.csv", "r");
    if (arquivo == NULL) {
        return 0;
    }

    char line[300];
    int count = 0;
    char cpfClean[12];
    cleanCpf(cpf, cpfClean);

    fgets(line, sizeof(line), arquivo);

    while (fgets(line, sizeof(line), arquivo) != NULL){
        char lineCopy[300];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';

        char *placa = strtok(lineCopy, ",");
        char *modelo = strtok(NULL, ",");
        char *anoStr = strtok(NULL, ",");
        char *cpfVeiculo = strtok(NULL, ",");

        if (cpfVeiculo != NULL) {
            char cpfVeiculoClean[12];
            cleanCpf(cpfVeiculo, cpfVeiculoClean);

            if (strcmp(cpfVeiculoClean, cpfClean) == 0) {
                count++;
            }
        }
    }
    fclose(arquivo);
    return count;

};

struct ordemServico* searchOrderById(int id) {
    FILE *arquivo = fopen("ordens_servico.csv", "r");
    if (arquivo == NULL) {
        return NULL;
    }
    
    char line[600];
    
    fgets(line, sizeof(line), arquivo);
    
    struct ordemServico *ordem = malloc(sizeof(struct ordemServico));
    if (ordem == NULL) {
        fclose(arquivo);
        return NULL;
    }
    
    while (fgets(line, sizeof(line), arquivo) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        char *placa = strtok(line, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");
        
        if (idStr != NULL) {
            int currentID = atoi(idStr);
            
            if (currentID == id) {
                if (descricao[0] == '"' && descricao[strlen(descricao)-1] == '"') {
                    memmove(descricao, descricao + 1, strlen(descricao) - 2);
                    descricao[strlen(descricao) - 2] = '\0';
                }
                
                strcpy(ordem->Placa, placa);
                ordem->status = atoi(statusStr);
                ordem->id = currentID;
                strcpy(ordem->DescProblem, descricao);
                strcpy(ordem->DataEntrada, data);
                
                fclose(arquivo);
                return ordem;
            }
        }
    }
    
    fclose(arquivo);
    free(ordem);
    return NULL;
}

void escreverRelatorio(const char* nomeArquivo, const char* conteudo) {
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de relatório: %s\n", nomeArquivo);
        return;
    }
    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}

//funções principais

void AddCliente (){
    char CPFForm[15];
    struct Cliente AddC;
    int c;
    
    printf("=== CADASTRO DE CLIENTE ===\n");
    do {
    printf("Insira o Nome: ");    
    fgets(AddC.Nome, sizeof(AddC.Nome), stdin);
    AddC.Nome[strcspn(AddC.Nome, "\n")] = '\0';
    } while (strlen(AddC.Nome) == 0);

    while (true){
        printf("Insira o CPF: ");
        fgets(AddC.CPF, sizeof(AddC.CPF), stdin);
        AddC.CPF[strcspn(AddC.CPF, "\n")] = '\0';
        if (!VeriCpf(AddC.CPF)){
            printf("Err CPF invalido, tente novamente\n");
            continue;
        }
        if (CpfExist(AddC.CPF)){
            printf("Err CPF ja cadastrado, tente novamente\n");
            continue;
        }
            formataCpf(AddC.CPF, CPFForm);
            strcpy(AddC.CPF, CPFForm);
            break;
    }
    while (true){
        printf("Insira o Telefone: ");
        fgets(AddC.Cell, sizeof(AddC.Cell), stdin);
        AddC.Cell[strcspn(AddC.Cell, "\n")] = '\0';

            if (VeriCell(AddC.Cell)){
                AddC.Cell[strcspn(AddC.Cell, "\n")] = '\0';
                break;
            }
            else{
                printf("erro ao verificar o Telefone, tente novamente\n");
            }
    }
    FILE *arquivo = fopen("clientes.csv", "a");
    
    if (arquivo == NULL) {
        printf("Erro no AddCliente \n");
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) {
    fprintf(arquivo, "Nome,CPF,Telefone\n");
}
   
    fseek(arquivo, 0, SEEK_END);
    fprintf(arquivo, "%s,%s,%s\n", 
            AddC.Nome,
            AddC.CPF,
            AddC.Cell
        );
        fclose(arquivo);
        printf("Cliente %s Adicionado com Sucesso\n", AddC.Nome);
        while ((c = getchar()) != '\n' && c != EOF);
};

void RemoveCliente() {
    char cpf[15];
    int c;
    
    printf("=== REMOVER CLIENTE ===\n");
    
    printf("Digite o CPF do cliente a ser removido:\n  ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    
    if (!VeriCpf(cpf)) {
        printf("CPF inválido!\n");
        return;
    }
    
    struct Cliente *cliente = searchPropByCpf(cpf);
    if (cliente == NULL) {
        printf("Cliente com CPF %s não encontrado!\n", cpf);
        return;
    }
    
    int numVeiculos = countVeiculos(cpf);
    if (numVeiculos > 0) {
        printf("\n❌ ERRO: Não é possível remover o cliente %s!\n", cliente->Nome);
        printf("Motivo: Existem %d veículo(s) cadastrado(s) para este cliente.\n", numVeiculos);
        printf("Transfira ou remova os veículos primeiro.\n");
        free(cliente);
        return;
    }
    
    printf("\n=== CLIENTE ENCONTRADO ===\n");
    printf("Nome: %s\n", cliente->Nome);
    printf("CPF: %s\n", cliente->CPF);
    printf("Telefone: %s\n", cliente->Cell);
    printf("Veículos associados: %d\n", numVeiculos);
    
    printf("\nTem certeza que deseja remover este cliente? (S/N): ");
    char confirmacao = getchar();
    while (getchar() != '\n');
    
    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Remoção cancelada.\n");
        free(cliente);
        return;
    }
    
    FILE *arquivoClientesOrig = fopen("clientes.csv", "r");
    if (arquivoClientesOrig == NULL) {
        printf("Erro ao abrir arquivo de clientes!\n");
        free(cliente);
        return;
    }
    
    FILE *arquivoClientesTemp = fopen("temp_clientes.csv", "w");
    if (arquivoClientesTemp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivoClientesOrig);
        free(cliente);
        return;
    }
    
    char line[300];
    bool found = false;

    if (fgets(line, sizeof(line), arquivoClientesOrig) != NULL) {
        fprintf(arquivoClientesTemp, "%s", line);
    }
    
    while (fgets(line, sizeof(line), arquivoClientesOrig) != NULL) {
        char lineCopy[300];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *nome = strtok(lineCopy, ",");
        char *cpfArquivo = strtok(NULL, ",");
        char *telefone = strtok(NULL, ",");
        
        if (cpfArquivo != NULL) {
            char cpfArquivoClean[12];
            char cpfClean[12];
            cleanCpf(cpfArquivo, cpfArquivoClean);
            cleanCpf(cpf, cpfClean);
            
            if (strcmp(cpfArquivoClean, cpfClean) == 0) {
                found = true;

            } else {
                fprintf(arquivoClientesTemp, "%s", line);
            }
        } else {
            fprintf(arquivoClientesTemp, "%s", line);
        }
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    fclose(arquivoClientesOrig);
    fclose(arquivoClientesTemp);
    
    if (!found) {
        printf("Erro: Cliente não encontrado no arquivo!\n");
        remove("temp_clientes.csv");
        free(cliente);
        return;
    }
    
    remove("clientes.csv");
    rename("temp_clientes.csv", "clientes.csv");
    
    printf("Cliente %s removido com sucesso!\n", cliente->Nome);
    free(cliente);
};

void EditCliente(){
    char cpf[15];
    char line[300];

    printf("=== EDITAR CLIENTE ===\n");

    printf("Digite o CPF do cliente a ser editado:\n  ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';
    
    if (!VeriCpf(cpf)) {
        printf("CPF inválido!\n");
        return;
    }
    
    struct Cliente *cliente = searchPropByCpf(cpf);
    if (cliente == NULL) {
        printf("Cliente com CPF %s não encontrado!\n", cpf);
        return;
    }

    printf("\n=== CLIENTE ENCONTRADO ===\n");
    printf("Nome atual: %s\n", cliente->Nome);
    printf("CPF: %s\n", cliente->CPF); 
    printf("Telefone atual: %s\n", cliente->Cell);

    struct Cliente clienteEditado;
    strcpy(clienteEditado.CPF, cliente->CPF);

    printf("\nNovo nome (atual: %s) [Enter para manter]: ", cliente->Nome);
    fgets(clienteEditado.Nome, sizeof(clienteEditado.Nome), stdin);
    clienteEditado.Nome[strcspn(clienteEditado.Nome, "\n")] = '\0';
    if (strlen(clienteEditado.Nome) == 0) {
        strcpy(clienteEditado.Nome, cliente->Nome);
    }

    printf("Novo telefone (atual: %s) [Enter para manter]: ", cliente->Cell);
    fgets(clienteEditado.Cell, sizeof(clienteEditado.Cell), stdin);
    clienteEditado.Cell[strcspn(clienteEditado.Cell, "\n")] = '\0';
    if (strlen(clienteEditado.Cell) == 0) {
        strcpy(clienteEditado.Cell, cliente->Cell); 
    } else {
        if (!VeriCell(clienteEditado.Cell)) {
            printf("Telefone inválido! Mantendo telefone atual.\n");
            strcpy(clienteEditado.Cell, cliente->Cell);
        }
    }

    if (strcmp(clienteEditado.Nome, cliente->Nome) == 0 && 
        strcmp(clienteEditado.Cell, cliente->Cell) == 0) {
        printf("Nenhuma alteração realizada.\n");
        free(cliente);
        return;
    }

     printf("\n=== ALTERAÇÕES ===\n");
    if (strcmp(clienteEditado.Nome, cliente->Nome) != 0) {
        printf("Nome: %s -> %s\n", cliente->Nome, clienteEditado.Nome);
    }

    if (strcmp(clienteEditado.Cell, cliente->Cell) != 0) {
        printf("Telefone: %s -> %s\n", cliente->Cell, clienteEditado.Cell);
    }
    
    printf("\nConfirmar edição? (S/N): ");
    char confirmacao = getchar();
    while (getchar() != '\n');
    
    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Edição cancelada.\n");
        free(cliente);
        return;
    }

        FILE *arquivoOriginal = fopen("clientes.csv", "r");
    if (arquivoOriginal == NULL) {
        printf("Erro ao abrir arquivo de clientes!\n");
        free(cliente);
        return;
    }
    
    FILE *arquivoTemp = fopen("temp_clientes.csv", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivoOriginal);
        free(cliente);
        return;
    }
    
    if (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        fprintf(arquivoTemp, "%s", line);
    }
    
    bool found = false;
    while (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        char lineCopy[300];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *nome = strtok(lineCopy, ",");
        char *cpfArquivo = strtok(NULL, ",");
        char *telefone = strtok(NULL, ",");
        
        if (cpfArquivo != NULL) {
            char cpfArquivoClean[12];
            char cpfClean[12];
            cleanCpf(cpfArquivo, cpfArquivoClean);
            cleanCpf(cpf, cpfClean);
            
            if (strcmp(cpfArquivoClean, cpfClean) == 0) {
                found = true;
                fprintf(arquivoTemp, "%s,%s,%s\n",
                        clienteEditado.Nome,
                        clienteEditado.CPF,
                        clienteEditado.Cell);
            } else {
                fprintf(arquivoTemp, "%s", line);
            }
        } else {
            fprintf(arquivoTemp, "%s", line);
        }
    }
    
    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    
    if (!found) {
        printf("Erro: Cliente não encontrado no arquivo!\n");
        remove("temp_clientes.csv");
        free(cliente);
        return;
    }

    if (remove("clientes.csv") != 0) {
        printf("Erro ao remover arquivo original!\n");
        free(cliente);
        return;
    }
    
    if (rename("temp_clientes.csv", "clientes.csv") != 0) {
        printf("Erro ao renomear arquivo temporário!\n");
        free(cliente);
        return;
    }
    
    printf("Cliente editado com sucesso!\n");
    printf("Novos dados: %s - %s - %s\n", 
           clienteEditado.Nome, clienteEditado.CPF, clienteEditado.Cell);
    
    free(cliente);
};

void AddVeiculo (){
    int c;
    char ComperCpf[15];
    struct Cliente *AddP = NULL;
    struct Veiculo AddV;

    printf("Informe o CPF do proprietario: ");
    scanf("%14s", ComperCpf);
    while ((c = getchar()) != '\n' && c != EOF);

    AddP = searchPropByCpf(ComperCpf);

    if(AddP == NULL){
        printf("Proprietario não encontrado!\n");
        return;
    };

    printf("Proprietario encontrado!\n");

    strcpy(AddV.propCpf, AddP->CPF);

    while ((c = getchar()) != '\n' && c != EOF);
    
    do{
        printf("Placa: ");
        fgets(AddV.Placa, sizeof(AddV.Placa), stdin);
        AddV.Placa[strcspn(AddV.Placa, "\n")] = '\0';
        for(int i = 0; AddV.Placa[i]; i++) {
            AddV.Placa[i] = toupper(AddV.Placa[i]);
        }
        if(!VeriPlaca(AddV.Placa)) {
            printf("Placa inválida, tente novamente!\n");
            continue;
        }
        if (PlacaExist(AddV.Placa)) {
            printf("Placa ja cadastrada, tente novamente!\n");
            continue;
        }

            break;
    } while(true);

    do {
        printf("Modelo: ");
        fgets(AddV.modelo, sizeof(AddV.modelo), stdin);
        AddV.modelo[strcspn(AddV.modelo, "\n")] = '\0';
    }while (strlen(AddV.modelo) == 0);

    do{
        printf("Ano: ");
        scanf("%d", &AddV.ano);
        while ((c = getchar()) != '\n' && c != EOF);
    }while(AddV.ano >2026 || AddV.ano<1886);

     FILE *arquivo_veiculos = fopen("veiculos.csv", "a");
    if (arquivo_veiculos == NULL) {
        printf("Erro em AddCarro: abrir arquivo veiculos.csv\n");
        free(AddP);
        return;   
    }

    fseek(arquivo_veiculos, 0, SEEK_END);
    if (ftell(arquivo_veiculos) == 0) {
        fprintf(arquivo_veiculos, "Placa,Modelo,Ano,CPF\n");
    }
    fprintf(arquivo_veiculos, "%s,%s,%d,%s\n",
            AddV.Placa,AddV.modelo,AddV.ano,
            AddV.propCpf);

    fclose(arquivo_veiculos);

    printf("Veículo cadastrado com sucesso!\n");
    free(AddP);
    while ((c = getchar()) != '\n' && c != EOF);
};

void RemoveVeiculo() {
    char placa[8];
    char line[300];
    bool found = false;
    int c;
    
    printf("=== REMOVER VEICULO ===\n");
    
    ListVeiculos();
    
    printf("\nDigite a placa do veiculo a ser removido: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';
    
    for(int i = 0; placa[i]; i++) {
        placa[i] = toupper(placa[i]);
    }
    
    struct Veiculo *veiculo = searchVeiByPlaca(placa);
    if (veiculo == NULL) {
        printf("Veiculo com placa %s nao encontrado!\n", placa);
        return;
    }
    
    FILE *arquivoOrdens = fopen("ordens_servico.csv", "r");
    bool temOrdens = false;
    
    if (arquivoOrdens != NULL) {
        char linhaOrdem[600];
        fgets(linhaOrdem, sizeof(linhaOrdem), arquivoOrdens); // Pular cabeçalho
        
        while (fgets(linhaOrdem, sizeof(linhaOrdem), arquivoOrdens) != NULL) {
            char linhaCopy[600];
            strcpy(linhaCopy, linhaOrdem);
            linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
            
            char *placaOrdem = strtok(linhaCopy, ",");
            
            if (placaOrdem != NULL && strcmp(placaOrdem, placa) == 0) {
                temOrdens = true;
                break;
            }
        }
        fclose(arquivoOrdens);
    }
    
    if (temOrdens) {
        printf("\nATENCAO: Este veiculo possui ordens de servico!\n");
        printf("Nao e possivel remover veiculos com historico de servicos.\n");
        free(veiculo);
        return;
    }
    
    printf("\n=== VEICULO ENCONTRADO ===\n");
    printf("Placa: %s\n", veiculo->Placa);
    printf("Modelo: %s\n", veiculo->modelo);
    printf("Ano: %d\n", veiculo->ano);
    printf("CPF Proprietario: %s\n", veiculo->propCpf);
    
    struct Cliente *proprietario = searchPropByCpf(veiculo->propCpf);
    if (proprietario != NULL) {
        printf("Proprietario: %s\n", proprietario->Nome);
        free(proprietario);
    }
    
    while (getchar() != '\n'); // Limpar buffer
    printf("\nTem certeza que deseja remover este veiculo? (S/N): ");
    char confirmacao = getchar();
    while (getchar() != '\n'); // Limpar buffer
    
    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Remocao cancelada.\n");
        free(veiculo);
        return;
    }

    FILE *arquivoOriginal = fopen("veiculos.csv", "r");
    if (arquivoOriginal == NULL) {
        printf("Erro ao abrir arquivo de veiculos!\n");
        free(veiculo);
        return;
    }
    
    FILE *arquivoTemp = fopen("temp_veiculos.csv", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arquivoOriginal);
        free(veiculo);
        return;
    }
    
    if (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        fprintf(arquivoTemp, "%s", line);
    }
    
    while (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        char lineCopy[300];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *placaArquivo = strtok(lineCopy, ",");
        
        if (placaArquivo != NULL) {
            char placaArqUpper[8];
            strcpy(placaArqUpper, placaArquivo);
            for(int i = 0; placaArqUpper[i]; i++) {
                placaArqUpper[i] = toupper(placaArqUpper[i]);
            }
            
            if (strcmp(placaArqUpper, placa) == 0) {
                found = true;
            } else {
                fprintf(arquivoTemp, "%s", line);
            }
        } else {
            fprintf(arquivoTemp, "%s", line);
        }
    }
    
    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    
    if (!found) {
        printf("Erro: Veiculo nao encontrado no arquivo!\n");
        remove("temp_veiculos.csv");
        free(veiculo);
        return;
    }
    
    if (remove("veiculos.csv") != 0) {
        printf("Erro ao remover arquivo original!\n");
        free(veiculo);
        return;
    }
    
    if (rename("temp_veiculos.csv", "veiculos.csv") != 0) {
        printf("Erro ao renomear arquivo temporario!\n");
        free(veiculo);
        return;
    }
    
    printf("Veiculo %s removido com sucesso!\n", placa);
    free(veiculo);
    while ((c = getchar()) != '\n' && c != EOF);
};

void TransfVeiculo() {
    char placa[8];
    char novoCpf[15];
    int c;
    
    printf("=== TRANSFERIR VEICULO (MUDAR PROPRIETARIO) ===\n");
    

    ListVeiculos();
    
    printf("\nDigite a placa do veiculo a ser transferido: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';
    
    for(int i = 0; placa[i]; i++) {
        placa[i] = toupper(placa[i]);
    }

    struct Veiculo *veiculo = searchVeiByPlaca(placa);
    if (veiculo == NULL) {
        printf("Veiculo com placa %s nao encontrado!\n", placa);
        return;
    }
    
    printf("\n=== VEICULO ENCONTRADO ===\n");
    printf("Placa: %s\n", veiculo->Placa);
    printf("Modelo: %s\n", veiculo->modelo);
    printf("Ano: %d\n", veiculo->ano);
    printf("CPF Proprietario Atual: %s\n", veiculo->propCpf);
    
    struct Cliente *proprietarioAtual = searchPropByCpf(veiculo->propCpf);
    if (proprietarioAtual != NULL) {
        printf("Proprietario Atual: %s\n", proprietarioAtual->Nome);
        free(proprietarioAtual);
    }
    while ((c = getchar()) != '\n' && c != EOF);
    printf("\nDigite o CPF do novo proprietario: ");
    fgets(novoCpf, sizeof(novoCpf), stdin);
    novoCpf[strcspn(novoCpf, "\n")] = '\0';
    
    if (!VeriCpf(novoCpf)) {
        printf("CPF invalido!\n");
        free(veiculo);
        return;
    }
    
    struct Cliente *novoProprietario = searchPropByCpf(novoCpf);
    if (novoProprietario == NULL) {
        printf("Novo proprietario nao encontrado! Cadastre-o primeiro.\n");
        free(veiculo);
        return;
    }
    
    printf("Novo Proprietario: %s\n", novoProprietario->Nome);
    
    printf("\nConfirmar transferencia do veiculo %s para %s? (S/N): ", 
           veiculo->Placa, novoProprietario->Nome);
    char confirmacao = getchar();
    while (getchar() != '\n');
    
    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Transferencia cancelada.\n");
        free(veiculo);
        free(novoProprietario);
        return;
    }
    
    FILE *arquivoOriginal = fopen("veiculos.csv", "r");
    if (arquivoOriginal == NULL) {
        printf("Erro ao abrir arquivo de veiculos!\n");
        free(veiculo);
        free(novoProprietario);
        return;
    }
    
    FILE *arquivoTemp = fopen("temp_veiculos.csv", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arquivoOriginal);
        free(veiculo);
        free(novoProprietario);
        return;
    }
    
    char line[300];
    if (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        fprintf(arquivoTemp, "%s", line);
    }
    
    bool found = false;
    while (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        char lineCopy[300];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *placaArquivo = strtok(lineCopy, ",");
        char *modelo = strtok(NULL, ",");
        char *anoStr = strtok(NULL, ",");
        char *cpfAtual = strtok(NULL, ",");
        
        if (placaArquivo != NULL) {
            char placaArqUpper[8];
            strcpy(placaArqUpper, placaArquivo);
            for(int i = 0; placaArqUpper[i]; i++) {
                placaArqUpper[i] = toupper(placaArqUpper[i]);
            }
            
            if (strcmp(placaArqUpper, placa) == 0) {
                found = true;

                char novoCpfFormatado[15];
                formataCpf(novoCpf, novoCpfFormatado);
                

                fprintf(arquivoTemp, "%s,%s,%s,%s\n",
                        placaArquivo,
                        modelo,
                        anoStr,
                        novoCpfFormatado);
            } else {

                fprintf(arquivoTemp, "%s", line);
            }
        } else {

            fprintf(arquivoTemp, "%s", line);
        }
    }
    
    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    
    if (!found) {
        printf("Erro: Veiculo nao encontrado no arquivo!\n");
        remove("temp_veiculos.csv");
        free(veiculo);
        free(novoProprietario);
        return;
    }
    

    if (remove("veiculos.csv") != 0) {
        printf("Erro ao remover arquivo original!\n");
        free(veiculo);
        free(novoProprietario);
        return;
    }
    
    if (rename("temp_veiculos.csv", "veiculos.csv") != 0) {
        printf("Erro ao renomear arquivo temporario!\n");
        free(veiculo);
        free(novoProprietario);
        return;
    }
    
    printf("Veiculo %s transferido com sucesso!\n", placa);
    printf("Novo proprietario: %s (%s)\n", novoProprietario->Nome, novoCpf);
    
    free(veiculo);
    free(novoProprietario);
    while ((c = getchar()) != '\n' && c != EOF);
};

void AddService(){
    int c;
    char comperPlaca[8];
    struct Veiculo *AddV = NULL;
    struct ordemServico AddS;
    char dataInput[11];

    printf("Informe a placa do veiculo: ");
    fgets(comperPlaca, sizeof(comperPlaca), stdin);
    comperPlaca[strcspn(comperPlaca, "\n")] = '\0';

    for(int i = 0; comperPlaca[i]; i++) {
        comperPlaca[i] = toupper(comperPlaca[i]);
    }
    
    AddV = (searchVeiByPlaca(comperPlaca));

    if(AddV == NULL){
        printf("Veiculo não encontrado!\n");
        return;
    };

    printf("Veiculo encontrado!\n");

    strcpy(AddS.Placa, AddV->Placa);
    while ((c = getchar()) != '\n' && c != EOF);

    do {
            printf("Digite a data (DD/MM/AAAA): ");
            fgets(dataInput, sizeof(dataInput), stdin);
            dataInput[strcspn(dataInput, "\n")] = '\0';
            
            if (VeriDate(dataInput)) {
                strcpy(AddS.DataEntrada, dataInput);
                break;
            } else {
                printf("Data invalida! Use o formato DD/MM/AAAA\n");
            }
        } while (true);

        AddS.status = AGUARDANDO_AVALIACAO;

    do{
        printf("Descricao do problema: ");
        fgets(AddS.DescProblem, sizeof(AddS.DescProblem), stdin);
        AddS.DescProblem[strcspn(AddS.DescProblem, "\n")] = '\0';
}while (strlen(AddS.DescProblem) == 0);

    int lastID = getLastOrderID();
    AddS.id = lastID + 1;

 FILE *arquivo = fopen("ordens_servico.csv", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de ordens de serviço!\n");
        free(AddV);
        return;
    }
    
    fseek(arquivo, 0, SEEK_END);
    if (ftell(arquivo) == 0) {
        fprintf(arquivo, "Placa,Status,ID,Descricao,DataEntrada\n");
    }
    
    fprintf(arquivo, "%s,%d,%d,\"%s\",%s\n",
            AddS.Placa,
            AddS.status,
            AddS.id,
            AddS.DescProblem,
            AddS.DataEntrada);
    
    fclose(arquivo);
    
    printf("\n=== ORDEM DE SERVICO CADASTRADA ===\n");
    printf("ID: %d\n", AddS.id);
    printf("Placa: %s\n", AddS.Placa);
    printf("Data: %s\n", AddS.DataEntrada);
    printf("Status: Aguardando Avaliacao\n");
    printf("Descricao: %s\n", AddS.DescProblem);
    
    free(AddV);
};

void RemoveService() {
    int idToRemove;
    char line[600];
    bool found = false;
    
    printf("=== REMOVER ORDEM DE SERVICO ===\n");
    
    FILE *arquivoLista = fopen("ordens_servico.csv", "r");
    if (arquivoLista == NULL) {
        printf("Nenhuma ordem de servico cadastrada!\n");
        return;
    }
    
    printf("\n=== ORDENS EXISTENTES ===\n");
    char header[600];
    fgets(header, sizeof(header), arquivoLista); 
    
    int count = 0;
    while (fgets(line, sizeof(line), arquivoLista) != NULL) {
        char lineCopy[600];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *placa = strtok(lineCopy, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");
        
        if (idStr) {
            int currentID = atoi(idStr);
            int status = atoi(statusStr);
            char *statusText;
            
            switch(status) {
                case 0: statusText = "Aguardando"; break;
                case 1: statusText = "Em Reparo"; break;
                case 2: statusText = "Finalizado"; break;
                case 3: statusText = "Entregue"; break;
                default: statusText = "Desconhecido";
            }
            
            printf("ID: %-3d | Placa: %-7s | Status: %-12s | Data: %s\n", 
                   currentID, placa, statusText, data);
            count++;
        }
    }
    fclose(arquivoLista);
    
    if (count == 0) {
        printf("Nenhuma ordem de servico encontrada.\n");
        return;
    }
    
    printf("\nDigite o ID da ordem de servico a ser removida: ");
    
    if (scanf("%d", &idToRemove) != 1) {
        printf("ID invalido!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    FILE *arquivoOriginal = fopen("ordens_servico.csv", "r");
    if (arquivoOriginal == NULL) {
        printf("Erro: Nenhuma ordem de servico cadastrada!\n");
        return;
    }
    
    FILE *arquivoTemp = fopen("temp_ordens.csv", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(arquivoOriginal);
        return;
    }
    
    // Copiar cabeçalho
    if (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        fprintf(arquivoTemp, "%s", line);
    }
    
    // Processar cada linha
    while (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        char lineCopy[600];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *placa = strtok(lineCopy, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");
        
        if (idStr != NULL) {
            int currentID = atoi(idStr);
            
            if (currentID == idToRemove) {
                found = true;
                printf("\n=== ORDEM ENCONTRADA ===\n");
                printf("ID: %d\n", currentID);
                printf("Placa: %s\n", placa);
                printf("Status: %s\n", statusStr);
                printf("Descricao: %s\n", descricao);
                printf("Data: %s\n", data);
                
                printf("\nTem certeza que deseja remover esta ordem? (S/N): ");
                char confirmacao = getchar();
                while (getchar() != '\n');
                
                if (confirmacao == 'S' || confirmacao == 's') {
                    printf("Ordem removida com sucesso!\n");
                    // Não copia esta linha para o arquivo temporário = REMOÇÃO
                } else {
                    // Copia a linha normalmente (usuário cancelou)
                    fprintf(arquivoTemp, "%s", line);
                    printf("Remocao cancelada.\n");
                }
            } else {
                // Copia a linha normalmente (não é o ID procurado)
                fprintf(arquivoTemp, "%s", line);
            }
        } else {
            // Linha inválida, copia de qualquer forma
            fprintf(arquivoTemp, "%s", line);
        }
    }
    
    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    
    if (!found) {
        printf("Ordem de servico com ID %d nao encontrada!\n", idToRemove);
        remove("temp_ordens.csv");
        return;
    }
    
    // Substituir arquivo original pelo temporário
    if (remove("ordens_servico.csv") != 0) {
        printf("Erro ao remover arquivo original!\n");
        return;
    }
    
    if (rename("temp_ordens.csv", "ordens_servico.csv") != 0) {
        printf("Erro ao renomear arquivo temporario!\n");
        return;
    }
    
    // RENUMERAR OS IDs RESTANTES
    printf("Renumerando IDs...\n");
    RenumberOrderIDs();
    printf("IDs renumerados com sucesso!\n");
};

void EditService() {
    int id;
    char line[600];
    
    printf("=== EDITAR ORDEM DE SERVIÇO ===\n");
    
    
    
    printf("\nDigite o ID da ordem de serviço a ser editada: ");
    if (scanf("%d", &id) != 1) {
        printf("ID inválido!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); // Limpar buffer
    
    // Buscar ordem de serviço pelo ID
    struct ordemServico *ordem = searchOrderById(id);
    if (ordem == NULL) {
        printf("Ordem de serviço com ID %d não encontrada!\n", id);
        return;
    }
    
    printf("\n=== ORDEM DE SERVIÇO ENCONTRADA ===\n");
    printf("ID: %d\n", ordem->id);
    printf("Placa: %s\n", ordem->Placa);
    printf("Data de Entrada: %s\n", ordem->DataEntrada);
    printf("Status atual: ");
    switch(ordem->status) {
        case AGUARDANDO_AVALIACAO: printf("Aguardando Avaliação\n"); break;
        case EM_REPARO: printf("Em Reparo\n"); break;
        case FINALIZADO: printf("Finalizado\n"); break;
        case ENTREGUE: printf("Entregue\n"); break;
        default: printf("Desconhecido\n");
    }
    printf("Descrição atual: %s\n", ordem->DescProblem);
    
    struct ordemServico ordemEditada;
    // Copiar dados imutáveis
    ordemEditada.id = ordem->id;
    strcpy(ordemEditada.Placa, ordem->Placa);
    strcpy(ordemEditada.DataEntrada, ordem->DataEntrada);
    
    // Editar status
    printf("\n=== EDITAR STATUS ===\n");
    printf("0 - Aguardando Avaliação\n");
    printf("1 - Em Reparo\n");
    printf("2 - Finalizado\n");
    printf("3 - Entregue\n");
    printf("Novo status (atual: %d) [Enter para manter]: ", ordem->status);
    
    char statusStr[10];
    fgets(statusStr, sizeof(statusStr), stdin);
    if (strlen(statusStr) > 1) {
        int novoStatus = atoi(statusStr);
        if (novoStatus >= 0 && novoStatus <= 3) {
            ordemEditada.status = novoStatus;
        } else {
            printf("Status inválido! Mantendo status atual.\n");
            ordemEditada.status = ordem->status;
        }
    } else {
        ordemEditada.status = ordem->status; // Mantém o atual
    }
    
    // Editar descrição do problema
    printf("\nNova descrição do problema (atual: %s): ", ordem->DescProblem);
    fgets(ordemEditada.DescProblem, sizeof(ordemEditada.DescProblem), stdin);
    ordemEditada.DescProblem[strcspn(ordemEditada.DescProblem, "\n")] = '\0';
    if (strlen(ordemEditada.DescProblem) == 0) {
        strcpy(ordemEditada.DescProblem, ordem->DescProblem); // Mantém o atual
    }
    
    // Verificar se realmente houve mudança
    if (ordemEditada.status == ordem->status && 
        strcmp(ordemEditada.DescProblem, ordem->DescProblem) == 0) {
        printf("Nenhuma alteração realizada.\n");
        free(ordem);
        return;
    }
    
    printf("\n=== ALTERAÇÕES ===\n");
    if (ordemEditada.status != ordem->status) {
        printf("Status: %d -> %d\n", ordem->status, ordemEditada.status);
    }
    if (strcmp(ordemEditada.DescProblem, ordem->DescProblem) != 0) {
        printf("Descrição: %s -> %s\n", ordem->DescProblem, ordemEditada.DescProblem);
    }
    
    printf("\nConfirmar edição? (S/N): ");
    char confirmacao = getchar();
    while (getchar() != '\n');
    
    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Edição cancelada.\n");
        free(ordem);
        return;
    }
    
    // Abordagem simples: remover a ordem antiga e adicionar a nova
    FILE *arquivoOriginal = fopen("ordens_servico.csv", "r");
    if (arquivoOriginal == NULL) {
        printf("Erro ao abrir arquivo de ordens de serviço!\n");
        free(ordem);
        return;
    }
    
    FILE *arquivoTemp = fopen("temp_ordens.csv", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        fclose(arquivoOriginal);
        free(ordem);
        return;
    }
    
    // Copiar cabeçalho
    if (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        fprintf(arquivoTemp, "%s", line);
    }
    
    // Processar cada linha
    bool found = false;
    while (fgets(line, sizeof(line), arquivoOriginal) != NULL) {
        char lineCopy[600];
        strcpy(lineCopy, line);
        lineCopy[strcspn(lineCopy, "\n")] = '\0';
        
        char *placa = strtok(lineCopy, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");
        
        if (idStr != NULL) {
            int currentID = atoi(idStr);
            
            if (currentID == id) {
                found = true;
                // Escreve os dados atualizados
                if (strchr(ordemEditada.DescProblem, ',') != NULL) {
                    fprintf(arquivoTemp, "%s,%d,%d,\"%s\",%s\n",
                            ordemEditada.Placa,
                            ordemEditada.status,
                            ordemEditada.id,
                            ordemEditada.DescProblem,
                            ordemEditada.DataEntrada);
                } else {
                    fprintf(arquivoTemp, "%s,%d,%d,%s,%s\n",
                            ordemEditada.Placa,
                            ordemEditada.status,
                            ordemEditada.id,
                            ordemEditada.DescProblem,
                            ordemEditada.DataEntrada);
                }
            } else {
                // Copia a linha normalmente
                fprintf(arquivoTemp, "%s", line);
            }
        } else {
            fprintf(arquivoTemp, "%s", line);
        }
    }
    
    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    
    if (!found) {
        printf("Erro: Ordem de serviço não encontrada no arquivo!\n");
        remove("temp_ordens.csv");
        free(ordem);
        return;
    }
    
    // Substituir arquivo original pelo temporário
    if (remove("ordens_servico.csv") != 0) {
        printf("Erro ao remover arquivo original!\n");
        free(ordem);
        return;
    }
    
    if (rename("temp_ordens.csv", "ordens_servico.csv") != 0) {
        printf("Erro ao renomear arquivo temporário!\n");
        free(ordem);
        return;
    }
    
    printf("✅ Ordem de serviço editada com sucesso!\n");
    printf("Novos dados:\n");
    printf("ID: %d | Placa: %s | Status: ", ordemEditada.id, ordemEditada.Placa);
    switch(ordemEditada.status) {
        case AGUARDANDO_AVALIACAO: printf("Aguardando Avaliação"); break;
        case EM_REPARO: printf("Em Reparo"); break;
        case FINALIZADO: printf("Finalizado"); break;
        case ENTREGUE: printf("Entregue"); break;
    }
    printf(" | Data: %s\n", ordemEditada.DataEntrada);
    printf("Descrição: %s\n", ordemEditada.DescProblem);
    
    free(ordem);
};

//funções dos relatorios

void HistoricoServicosVeiculo() {
    char placa[8];
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("=== HISTÓRICO DE SERVIÇOS DO VEÍCULO ===\n");
    printf("Digite a placa do veículo: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    for(int i = 0; placa[i]; i++) {
        placa[i] = toupper(placa[i]);
    }

    // Limpar arquivo anterior
    remove("historico_veiculo.txt");
    
    // Escrever cabeçalho
    FILE *txtFile = fopen("historico_veiculo.txt", "w");
    if (txtFile == NULL) {
        printf("Erro ao criar arquivo de relatório!\n");
        return;
    }
    
    fprintf(txtFile, "Histórico de Serviços - Placa: %s\n", placa);
    fprintf(txtFile, "==========================================\n");
    fprintf(txtFile, "ID  | Data       | Status            | Descrição\n");
    fprintf(txtFile, "----|------------|-------------------|----------\n");
    fclose(txtFile);

    FILE *arquivo = fopen("ordens_servico.csv", "r");
    if (arquivo == NULL) {
        escreverRelatorio("historico_veiculo.txt", "Nenhuma ordem de serviço cadastrada!\n");
        printf("Nenhuma ordem de serviço cadastrada!\n");
        return;
    }

    char linha[600];
    int encontrou = 0;

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char linhaCopy[600];
        strcpy(linhaCopy, linha);
        linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
        
        char *placaArq = strtok(linhaCopy, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");

        if (placaArq != NULL && strcmp(placaArq, placa) == 0) {
            encontrou = 1;
            int status = atoi(statusStr);
            char *statusTexto;
            
            switch(status) {
                case 0: statusTexto = "Aguardando"; break;
                case 1: statusTexto = "Em Reparo"; break;
                case 2: statusTexto = "Finalizado"; break;
                case 3: statusTexto = "Entregue"; break;
                default: statusTexto = "Desconhecido";
            }
            
            char buffer[600];
            sprintf(buffer, "%-3s | %-10s | %-17s | %s\n", idStr, data, statusTexto, descricao);
            escreverRelatorio("historico_veiculo.txt", buffer);
        }
    }
    
    fclose(arquivo);
    
    if (!encontrou) {
        escreverRelatorio("historico_veiculo.txt", "Nenhum serviço encontrado para esta placa.\n");
    }
    
    printf("Relatório salvo em: historico_veiculo.txt\n");
    while ((c = getchar()) != '\n' && c != EOF);
}

void VeiculosPerCliente(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    char cpf[15];
    printf("=== VEÍCULOS POR CLIENTE ===\n");
    printf("Digite o CPF do cliente: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    if (!VeriCpf(cpf)) {
        printf("CPF inválido!\n");
        return;
    }

    struct Cliente *cliente = searchPropByCpf(cpf);
    if (cliente == NULL) {
        printf("Cliente não encontrado!\n");
        return;
    }

    remove("veiculos_cliente.txt");
    
    FILE *txtFile = fopen("veiculos_cliente.txt", "w");
    if (txtFile == NULL) {
        printf("Erro ao criar arquivo de relatório!\n");
        free(cliente);
        return;
    }

    fprintf(txtFile, "Veículos do Cliente: %s\n", cliente->Nome);
    fprintf(txtFile, "CPF: %s\n", cliente->CPF);
    fprintf(txtFile, "================================\n\n");
    fprintf(txtFile, "Placa   | Modelo               | Ano\n");
    fprintf(txtFile, "--------|----------------------|-----\n");
    fclose(txtFile);

    FILE *arquivo = fopen("veiculos.csv", "r");
    if (arquivo == NULL) {
        escreverRelatorio("veiculos_cliente.txt", "Nenhum veículo cadastrado!\n");
        free(cliente);
        return;
    }

    char linha[300];
    int encontrou = 0;

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char linhaCopy[300];
        strcpy(linhaCopy, linha);
        linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
        
        char *placa = strtok(linhaCopy, ",");
        char *modelo = strtok(NULL, ",");
        char *anoStr = strtok(NULL, ",");
        char *cpfArq = strtok(NULL, ",");

        if (cpfArq != NULL) {
            char cpfArqClean[12];
            char cpfClean[12];
            cleanCpf(cpfArq, cpfArqClean);
            cleanCpf(cpf, cpfClean);
            
            if (strcmp(cpfArqClean, cpfClean) == 0) {
                encontrou = 1;
                int ano = atoi(anoStr);
                char buffer[300];
                sprintf(buffer, "%-7s | %-20s | %-4d\n", placa, modelo, ano);
                escreverRelatorio("veiculos_cliente.txt", buffer);
            }
        }
    }
    
    fclose(arquivo);
    
    if (!encontrou) {
        escreverRelatorio("veiculos_cliente.txt", "Nenhum veículo encontrado para este cliente.\n");
    }
    
    free(cliente);
    printf("Relatório salvo em: veiculos_cliente.txt\n");
    while ((c = getchar()) != '\n' && c != EOF);
}

void RelatorioPerStatus(){
    int status;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("=== RELATÓRIO POR STATUS ===\n");
    printf("0 - Aguardando Avaliação\n");
    printf("1 - Em Reparo\n");
    printf("2 - Finalizado\n");
    printf("3 - Entregue\n");
    printf("Digite o status: ");
    
    if (scanf("%d", &status) != 1 || status < 0 || status > 3) {
        printf("Status inválido!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    char *statusTexto;
    switch(status) {
        case 0: statusTexto = "Aguardando Avaliação"; break;
        case 1: statusTexto = "Em Reparo"; break;
        case 2: statusTexto = "Finalizado"; break;
        case 3: statusTexto = "Entregue"; break;
        default: statusTexto = "Desconhecido";
    }

    remove("relatorio_status.txt");
    
    FILE *txtFile = fopen("relatorio_status.txt", "w");
    if (txtFile == NULL) {
        printf("Erro ao criar arquivo de relatório!\n");
        return;
    }

    fprintf(txtFile, "Relatório por Status: %s\n", statusTexto);
    fprintf(txtFile, "==============================\n");
    fprintf(txtFile, "ID  | Placa   | Data       | Descrição\n");
    fprintf(txtFile, "----|---------|------------|----------\n");
    fclose(txtFile);

    FILE *arquivo = fopen("ordens_servico.csv", "r");
    if (arquivo == NULL) {
        escreverRelatorio("relatorio_status.txt", "Nenhuma ordem de serviço cadastrada!\n");
        return;
    }

    char linha[600];
    bool encontrou = false;

    fgets(linha, sizeof(linha), arquivo);
    
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char linhaCopy[600];
        strcpy(linhaCopy, linha);
        linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
        
        char *placa = strtok(linhaCopy, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *data = strtok(NULL, ",");

        if (statusStr != NULL && atoi(statusStr) == status) {
            encontrou = true;
            char buffer[600];
            sprintf(buffer, "%-3s | %-7s | %-10s | %s\n", idStr, placa, data, descricao);
            escreverRelatorio("relatorio_status.txt", buffer);
        }
    }
    
    fclose(arquivo);
    
    if (!encontrou) {
        escreverRelatorio("relatorio_status.txt", "Nenhuma ordem encontrada com este status.\n");
    }
    
    printf("Relatório salvo em: relatorio_status.txt\n");
    while ((c = getchar()) != '\n' && c != EOF);
}

void ClientesRecorrentes() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("=== CLIENTES MAIS RECORRENTES ===\n");

    remove("clientes_recorrentes.txt");
    
    FILE *txtFile = fopen("clientes_recorrentes.txt", "w");
    if (txtFile == NULL) {
        printf("Erro ao criar arquivo de relatório!\n");
        return;
    }

    fprintf(txtFile, "Clientes Mais Recorrentes\n");
    fprintf(txtFile, "==========================\n\n");
    fclose(txtFile);
    
    FILE *arquivoClientes = fopen("clientes.csv", "r");
    FILE *arquivoOrdens = fopen("ordens_servico.csv", "r");
    FILE *arquivoVeiculos = fopen("veiculos.csv", "r");
    
    if (arquivoClientes == NULL || arquivoOrdens == NULL || arquivoVeiculos == NULL) {
        escreverRelatorio("clientes_recorrentes.txt", "Erro ao abrir arquivos necessários!\n");
        if (arquivoClientes) fclose(arquivoClientes);
        if (arquivoOrdens) fclose(arquivoOrdens);
        if (arquivoVeiculos) fclose(arquivoVeiculos);
        return;
    }

    typedef struct {
        char cpf[15];
        char nome[100];
        int count;
    } ClienteCount;

    ClienteCount clientes[1000];
    int numClientes = 0;

    // Ler clientes
    char linha[300];
    fgets(linha, sizeof(linha), arquivoClientes);
    
    while (fgets(linha, sizeof(linha), arquivoClientes) != NULL && numClientes < 1000) {
        char linhaCopy[300];
        strcpy(linhaCopy, linha);
        linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
        
        char *nome = strtok(linhaCopy, ",");
        char *cpf = strtok(NULL, ",");
        char *telefone = strtok(NULL, ",");
        
        if (nome && cpf) {
            strcpy(clientes[numClientes].nome, nome);
            strcpy(clientes[numClientes].cpf, cpf);
            clientes[numClientes].count = 0;
            numClientes++;
        }
    }
    fclose(arquivoClientes);

    // Contar ordens por cliente
    fgets(linha, sizeof(linha), arquivoOrdens);
    
    while (fgets(linha, sizeof(linha), arquivoOrdens) != NULL) {
        char linhaCopy[600];
        strcpy(linhaCopy, linha);
        linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
        
        char *placa = strtok(linhaCopy, ",");
        
        if (placa) {
            struct Veiculo *veiculo = searchVeiByPlaca(placa);
            if (veiculo != NULL) {
                for (int i = 0; i < numClientes; i++) {
                    char cpfVeiculoClean[12];
                    char cpfClienteClean[12];
                    cleanCpf(veiculo->propCpf, cpfVeiculoClean);
                    cleanCpf(clientes[i].cpf, cpfClienteClean);
                    
                    if (strcmp(cpfVeiculoClean, cpfClienteClean) == 0) {
                        clientes[i].count++;
                        break;
                    }
                }
                free(veiculo);
            }
        }
    }
    fclose(arquivoOrdens);
    fclose(arquivoVeiculos);

    // Ordenar clientes
    for (int i = 0; i < numClientes - 1; i++) {
        for (int j = i + 1; j < numClientes; j++) {
            if (clientes[i].count < clientes[j].count) {
                ClienteCount temp = clientes[i];
                clientes[i] = clientes[j];
                clientes[j] = temp;
            }
        }
    }

    // Escrever cabeçalho da tabela
    escreverRelatorio("clientes_recorrentes.txt", "Ranking de Clientes Mais Recorrentes:\n");
    escreverRelatorio("clientes_recorrentes.txt", "Pos | Nome                     | CPF           | Ordens\n");
    escreverRelatorio("clientes_recorrentes.txt", "----|--------------------------|---------------|-------\n");
    
    int maxExibir = (numClientes > 10) ? 10 : numClientes;
    for (int i = 0; i < maxExibir; i++) {
        if (clientes[i].count > 0) {
            char buffer[200];
            sprintf(buffer, "%-3d | %-24s | %-13s | %d\n", 
                   i + 1, clientes[i].nome, clientes[i].cpf, clientes[i].count);
            escreverRelatorio("clientes_recorrentes.txt", buffer);
        }
    }
    
    if (maxExibir == 0) {
        escreverRelatorio("clientes_recorrentes.txt", "Nenhuma ordem de serviço encontrada.\n");
    }
    
    printf("Relatório salvo em: clientes_recorrentes.txt\n");
    while ((c = getchar()) != '\n' && c != EOF);
}

void ServicePerData() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    char data[11];
    printf("=== ORDENS DE SERVIÇO POR DATA ===\n");
    printf("Digite a data (DD/MM/AAAA): ");
    fgets(data, sizeof(data), stdin);
    data[strcspn(data, "\n")] = '\0';

    if (!VeriDate(data)) {
        printf("Data inválida!\n");
        return;
    }

    remove("ordens_data.txt");
    
    FILE *txtFile = fopen("ordens_data.txt", "w");
    if (txtFile == NULL) {
        printf("Erro ao criar arquivo de relatório!\n");
        return;
    }
    
    fprintf(txtFile, "Ordens de Serviço - Data: %s\n", data);
    fprintf(txtFile, "================================\n");
    fprintf(txtFile, "ID  | Placa   | Status            | Descrição\n");
    fprintf(txtFile, "----|---------|-------------------|----------\n");
    fclose(txtFile);

    FILE *arquivo = fopen("ordens_servico.csv", "r");
    if (arquivo == NULL) {
        escreverRelatorio("ordens_data.txt", "Nenhuma ordem de serviço cadastrada!\n");
        printf("Nenhuma ordem de serviço cadastrada!\n");
        return;
    }

    char linha[600];
    int encontrou = 0;
    
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char linhaCopy[600];
        strcpy(linhaCopy, linha);
        linhaCopy[strcspn(linhaCopy, "\n")] = '\0';
        
        char *placa = strtok(linhaCopy, ",");
        char *statusStr = strtok(NULL, ",");
        char *idStr = strtok(NULL, ",");
        char *descricao = strtok(NULL, ",");
        char *dataArq = strtok(NULL, ",");

        if (dataArq != NULL && strcmp(dataArq, data) == 0) {
            encontrou = 1;
            int status = atoi(statusStr);
            char *statusTexto;
            
            switch(status) {
                case 0: statusTexto = "Aguardando"; break;
                case 1: statusTexto = "Em Reparo"; break;
                case 2: statusTexto = "Finalizado"; break;
                case 3: statusTexto = "Entregue"; break;
                default: statusTexto = "Desconhecido";
            }
            
            char buffer[600];
            sprintf(buffer, "%-3s | %-7s | %-17s | %s\n", idStr, placa, statusTexto, descricao);
            escreverRelatorio("ordens_data.txt", buffer);
        }
    }
    
    fclose(arquivo);
    
    if (!encontrou) {
        escreverRelatorio("ordens_data.txt", "Nenhuma ordem encontrada para esta data.\n");
    }
    
    printf("Relatório salvo em: ordens_data.txt\n");
    while ((c = getchar()) != '\n' && c != EOF);
}

//menus

void exibirMenu() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║        SISTEMA OFICINA - ROCK RURAL    ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. GESTÃO DE CLIENTES\n");
    printf("2. GESTÃO DE VEÍCULOS\n");
    printf("3. GESTÃO DE SERVIÇOS\n");
    printf("4. RELATÓRIOS\n");
    printf("5. SAIR\n");
    printf("\nEscolha uma opção: ");
};
void menuClientes() {
    int opcao;
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║            GESTÃO DE CLIENTES          ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("1. Adicionar Cliente\n");
        printf("2. Remover Cliente\n");
        printf("3. Editar Cliente\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("\nEscolha uma opção: ");
        
        scanf("%d", &opcao);
        while (getchar() != '\n'); 
        
        switch(opcao) {
            case 1:
                AddCliente();
                break;
            case 2:
                RemoveCliente();
                break;
            case 3:
                EditCliente();
                break;
            case 4:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);
}

void menuVeiculos() {
    int opcao;
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║            GESTÃO DE VEÍCULOS          ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("1. Adicionar Veículo\n");
        printf("2. Remover Veículo\n");
        printf("3. Transferir Veículo\n");
        printf("4. Listar Todos os Veículos\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("\nEscolha uma opção: ");
        
        scanf("%d", &opcao);
        while (getchar() != '\n'); 
        
        switch(opcao) {
            case 1:
                AddVeiculo();
                break;
            case 2:
                RemoveVeiculo();
                break;
            case 3:
                TransfVeiculo();
                break;
            case 4:
                ListVeiculos();
                break;
            case 5:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);
}

void menuServicos() {
    int opcao;
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║            GESTÃO DE SERVIÇOS          ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("1. Adicionar Ordem de Serviço\n");
        printf("2. Remover Ordem de Serviço\n");
        printf("3. Editar Ordem de Serviço\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("\nEscolha uma opção: ");
        
        scanf("%d", &opcao);
        while (getchar() != '\n'); 
        
        switch(opcao) {
            case 1:
                AddService();
                break;
            case 2:
                RemoveService();
                break;
            case 3:
                EditService();
                break;
            case 4:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);
}

void menuRelatorios() {
    int opcao;
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║               RELATÓRIOS               ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("1. Histórico de Serviços de um Veículo\n");
        printf("2. Ordens de Serviço por Data\n");
        printf("3. Veículos de um Cliente\n");
        printf("4. Relatório por Status\n");
        printf("5. Clientes Mais Recorrentes\n");
        printf("6. Voltar ao Menu Principal\n");
        printf("\nEscolha uma opção: ");
        
        scanf("%d", &opcao);
        while (getchar() != '\n'); 
        
        switch(opcao) {
            case 1:
                HistoricoServicosVeiculo();
                break;
            case 2:
                ServicePerData();
                break;
            case 3:
                VeiculosPerCliente();
                break;
            case 4:
                RelatorioPerStatus();
                break;
            case 5:
                ClientesRecorrentes();
                break;
            case 6:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);
}

int main(){
    int opcao;
    
    FILE *teste;
    
    teste = fopen("clientes.csv", "a");
    if (teste) fclose(teste);
    
    teste = fopen("veiculos.csv", "a");
    if (teste) fclose(teste);
    
    teste = fopen("ordens_servico.csv", "a");
    if (teste) fclose(teste);
    
    printf("BEM-VINDO AO SISTEMA OFICINA ROCK RURAL!\n");
    printf("Sistema inicializado com sucesso!\n");
    
    do {
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Digite um número.\n");
            while (getchar() != '\n'); 
            continue;
        }
        while (getchar() != '\n'); 
        
        switch(opcao) {
            case 1:
                menuClientes();
                break;
            case 2:
                menuVeiculos();
                break;
            case 3:
                menuServicos();
                break;
            case 4:
                menuRelatorios();
                break;
            case 5:
                printf("\nObrigado por usar o Sistema Oficina Rock Rural!\n");
                printf("Até a próxima!\n");
                break;
            default:
                printf("Opção inválida! Digite um número entre 1 e 5.\n");
        }
    } while (opcao != 5);
    
    return 0;
};

