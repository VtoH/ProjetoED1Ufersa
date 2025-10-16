╔══════════════════════════════════════════════════════╗
║       COMO USAR O SISTEMA DA OFICINA - ROCK RURAL    ║
╚══════════════════════════════════════════════════════╝

Ambiente de Desenvolvimento
Sistema Operacional: Manjaro Linux XFCE (base Arch)
Versão do SO: Mandriva (64 bits)
Compilador Utilizado: gcc
Versão mínima recomendada: GCC 9.0+
Padrão C utilizado: c99
Editor / IDE: Visual Studio Code (executado dentro da VM)
Máquina Virtual: Oracle VirtualBox
Configurações da VM:
    Memória RAM: 11.556 MB
    Nucleos: 4
    Armazenamento: 40 GB (arquivo Manjaro.vdi)
    Interface de Rede: Intel PRO/1000 MT Desktop (NAT)
    Som: ICH AC97
    EFI: Habilitado
    Aceleração: Paginação Aninhada + Paravirtualização KVM

!IMPORTANTE PARA O TESTE!

Capacidade máxima: ~1000 ordens de serviço (array estático)

verificações de Cliente:
    o CPF esta função verica APENAS a QUANTIDADE(11 inteiros) e o TIPO colocado.
    (Ignora . é -)
    Exemplos funcionais:
        666.666.666-66 é 11111111111
    Exemplos do que não funciona(teoricamente):
        666.666.AAA-66, 666.666.66-66 é 666.666.666-666
    o Numero de telefone é muito parecido, verifica o TIPO e a QUANTIDADE(10 ou 11 inteiros)
    (Ignora " ",-,(,))
    Exemplos funcionais:
        21212121212, (21)212121212 e etc
    Exemplos do que não funciona(teoricamente):
        21212121AAA, (21)2121212122121 é (21)21212

verificações de Veiculos:
    a placa possui uma verificação levemente mais robusta, ela verifica com base nos 2 
    sistemas de placas, o Merco-Sul e o nosso antigo sistema de placas.
    (Merco-Sul: LLL NLNN; Antigo: LLLNNNN)
    Exemplos funcionais:
        AAA1111 é AAA1A11
    Exemplos do que não funciona(teoricamente):
        AAA111A, AAAAAAA é 7777777

verificações de Serviços:
    a Data leva em conta coisas como ano bisexto é os dias do més é obriga o usuario a usar as / na data.
    Exemplos funcionais:
        12/12/2012 é 18/05/2004
    Exemplos do que não funciona(teoricamente):
        AA/12/2012, 12/12/1500 é 12/12/2026

ITENS IMUTÁVEIS NO SISTEMA

Cliente
CPF - Não pode ser alterado
otivos:
- Base Legal: O CPF é um identificador único e permanente por lei brasileira
- Integridade Referencial: Alterar o CPF quebraria o vínculo com todos os veículos e ordens de serviço associadas
- Consistência Histórica: Mantém a precisão dos relatórios e histórico de serviços
- Prevenção a Fraudes: Impede duplicação ou substituição de identidades

 Veículo
Placa e Modelo - Não podem ser alterados
Motivos:
- Identificação Única: A placa é o "RG" do veículo no sistema
- Característica Intrínseca: Modelo e ano são características de fabricação imutáveis
- Solução Elegante: Criamos a função "Transferir Veículo" para casos de venda

 Ordem de Serviço
ID - Não pode ser alterado
Motivos:
- Identificador Único: Garante que cada ordem tenha uma referência exclusiva
- Rastreabilidade: Permite localizar rapidamente qualquer serviço no histórico
- Integridade: Evita duplicações e conflitos no sistema
- Sequência Lógica: Mantém a ordem cronológica dos serviços

Data de Entrada - Não pode ser alterada
Motivos:
- Fluxo de Trabalho: Reflete o processo real da oficina

Arquivos:
    CSV's:
        cada "objeto" tem seu proprio CSV, desdo cliente ao serviço, se o csv não existir ele é criado dentro do proprio codigo.
    TXT's:
        unico para os relatorios, cada um deles e "alterado" apos o user pedir o relatorio novamente, mostrando APENAS as infos novas.

POSSIVEIS FALHAS:
    problemas no buffer:
        Apesar de não prejudicar de fato o sistema, em alguns testes depois de por exemplo adicionar um cliente, pos a mensagem de confirmação se era necessario apertar enter novamente, ACHO que arrumei.
    Problema em alocação de memoria:
        Não identifiquei mas é possivel que tenha, ate o momento não teve nenhum problema.

possiveis melhorias:
    Usar metodos do CUDA ou outra biblioteca para alocar um paralelismo, asim seria possivel fazer uma "Pre-lida" dos arquivos
    tornando o codigo mais eficiente, e interresante adicionar mesmo que seja a alocação pelas threads.

exemplo de como usar:
1. Menu 1 -> Opção 1 (Cadastrar Cliente)
   - Nome: Fred Jones
   - CPF: 123.456.789-00
   - Telefone: (11) 99999-9999

2. Menu 2 -> Opção 1 (Cadastrar Veículo)  
   - CPF: 123.456.789-00
   - Placa: SKB1D00
   - Modelo: Maquina de Misterios
   - Ano: 1970

3. Menu 3 -> Opção 1 (Abrir Ordem)
   - Placa: SKB1D00
   - Data: 15/10/2024
   - Problema: Biscoitos scooby no escapamento.
  
Desenvolvido por: Victor Hugo Lima Ferreira & Pedro Carvalho Pessoa;
Versão do código: 1.0
Data: Outubro de 2025