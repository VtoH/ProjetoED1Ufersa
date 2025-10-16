╔══════════════════════════════════════════════════════╗

║                🛠️ OFICINA - ROCK RURAL 🛠️             ║

╚══════════════════════╝

## Ambiente de Desenvolvimento
**Sistema Operacional:** Manjaro Linux XFCE (base Arch)  
**Versão do SO:** Mandriva (64 bits)  
**Compilador Utilizado:** gcc  
**Versão mínima recomendada:** GCC 9.0+  
**Padrão C utilizado:** c99  
**Editor / IDE:** Visual Studio Code (executado dentro da VM)  

**Máquina Virtual:** Oracle VirtualBox  
**Configurações da VM:**
- Memória RAM: 11.556 MB
- Nucleos: 4
- Armazenamento: 40 GB (arquivo Manjaro.vdi)
- Interface de Rede: Intel PRO/1000 MT Desktop (NAT)
- Som: ICH AC97
- EFI: Habilitado
- Aceleração: Paginação Aninhada + Paravirtualização KVM

## IMPORTANTE PARA O TESTE

**Capacidade máxima:** ~1000 ordens de serviço (array estático)

### verificações de Cliente:
- **CPF:** verifica APENAS a QUANTIDADE(11 inteiros) e o TIPO colocado (Ignora . é -)
  - ✅ **Funcionais:** 666.666.666-66 é 11111111111
  - ❌ **Não funcionam:** 666.666.AAA-66, 666.666.66-66 é 666.666.666-666

- **Telefone:** verifica o TIPO e a QUANTIDADE(10 ou 11 inteiros) (Ignora " ",-,(,))
  - ✅ **Funcionais:** 21212121212, (21)212121212 e etc
  - ❌ **Não funcionam:** 21212121AAA, (21)2121212122121 é (21)21212

### verificações de Veiculos:
- **Placa:** verificação com base nos 2 sistemas de placas (Merco-Sul: LLL NLNN; Antigo: LLLNNNN)
  - ✅ **Funcionais:** AAA1111 é AAA1A11
  - ❌ **Não funcionam:** AAA111A, AAAAAAA é 7777777

### verificações de Serviços:
- **Data:** considera ano bisexto e dias do més, obriga usar as / na data
  - ✅ **Funcionais:** 12/12/2012 é 18/05/2004
  - ❌ **Não funcionam:** AA/12/2012, 12/12/1500 é 12/12/2026

## ITENS IMUTÁVEIS NO SISTEMA

### Cliente
**CPF - Não pode mudar**

Porque:
- É o documento único de cada pessoa
- Não deixa criar CPF duplicado

### Veículo  
**Placa e Modelo - Não podem mudar**

Porque:
- A placa é a identidade do carro
- Modelo e ano são do jeito que veio de fábrica
- Se o carro for vendido, transfira para outro dono ou excluir o veiculo.

### Ordem de Serviço
**ID e Data- Não pode mudar**

Porque:
- Cada serviço tem seu ID
- Facilita encontrar serviços antigos
- Evita confusão com IDs iguais
- Mantém a ordem dos serviços
- Mostra quando o carro entrou na oficina

## Arquivos

**CSV's:**
- Cada "objeto" tem seu proprio CSV
- Se não existir, é criado automaticamente

**TXT's:**
- Único para os relatorios
- Mostra APENAS informações novas quando gerado novamente

## POSSIVEIS FALHAS

**Problemas no buffer:**
- As vezes precisa apertar Enter novamente depois de algumas mensagens

**Problema em alocação de memoria:**
- Não identificado até agora, mas possível

## POSSIVEIS MELHORIAS
- Usar paralelismo para "pre-ler" arquivos
- Adicionar threads para maior eficiência

## EXEMPLO DE COMO USAR

1. **Menu 1 -> Opção 1** (Cadastrar Cliente)
   - Nome: Fred Jones
   - CPF: 123.456.789-00
   - Telefone: (11) 99999-9999

2. **Menu 2 -> Opção 1** (Cadastrar Veículo)  
   - CPF: 123.456.789-00
   - Placa: SKB1D00
   - Modelo: Maquina de Misterios
   - Ano: 1970

3. **Menu 3 -> Opção 1** (Abrir Ordem)
   - Placa: SKB1D00
   - Data: 15/10/2024
   - Problema: Biscoitos scooby no escapamento

---

**Desenvolvido por:** Victor Hugo Lima Ferreira & Pedro Carvalho Pessoa  
**Versão do código:** 1.0  
**Data:** Outubro de 2025
