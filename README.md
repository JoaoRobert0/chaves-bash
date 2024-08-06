# ChavesBash

ChavesBash é um interpretador de comandos simples em C++ que oferece funcionalidades básicas de um shell. Ele permite executar comandos internos e externos, gerenciar o histórico de comandos e mudar o diretório de trabalho.

## Funcionalidades

- **Comandos Internos:**
  - `exit`: Encerra o interpretador.
  - `pwd`: Exibe o diretório de trabalho atual.
  - `clear`: Limpa a tela do terminal.
  - `cd [diretório]`: Muda o diretório de trabalho. Se nenhum diretório for fornecido, muda para o diretório inicial definido.

- **Histórico de Comandos:**
  - `history`: Mostra os últimos 10 comandos executados.
  - `history -c`: Limpa o histórico de comandos.
  - `history [offset]`: Executa o comando no histórico na posição `[offset]`. 

- **Comandos Externos:**
  - Executa comandos externos encontrados em `/bin/`.

## Compilação e Execução

Para compilar o projeto, use um compilador C++ como `g++`. No diretório do projeto, execute:

```sh
g++ -o ChavesBash.out main.cpp
```

Para executar o programa, use:

```sh
./ChavesBash.out
```

## Configuração
Antes de executar o programa, ajuste o caminho para o diretório inicial no código fonte:

```sh
const std::string initial_directory = "/home/joao";
```

Substitua __"/home/joao"__ pelo caminho desejado para o diretório inicial.