# Objetivo
Desenvolver um program em C, norma ansi, com a finalidade de calcular distâncias de rotas. As rotas são definidas através de uma sequência de nomes de localidades. Todas as possíveis localidades estão guardadas num ficheiro de texto (base de dados) cujo nome é localidades.txt. As rotas são especificadas num ficheiro de texto (rotas.txt) que o programa deverá processar.

Para mais informação consultar o enunciado do trabalho

# Lógica do programa
O programa foi desenvolvido seguindo a lógica de processamento dos ficheiro.
  1. Leitura dos dados das localidades implementando uma lista simplesmente ligada como estrutura de dados apra guardar a informação (nome, latitude e longitude).
  2. Processamento das rotas. Para tal abordou-se rota a rota tirando partido da estrutura pela qual estão descritas as rotas.

# Operações matemáticas essenciais
A distância foi calculada por uma aproximação simplificada da expressão onde se considera um sistema de eixos xy. Considera-se então um deslocamento segundo x outro segundo y. A distância é portanto dada pela raiz quadrada da soma dos quadrados dos deslocamentos segundo cada eixo. Considere-se que um ponto de referência tem longitude "lon_0" e latitude "lat_0" e um ponto perto da referência longitude "lon" e latitude "lat"
  1. x = R * cos(lon) sen(lat - lat_0) 
  2. y = R * tan(lon - lon_0)

Para se aplicar estas fórmuals foi preciso converter de graus para radianos as longitudes e latitudes

# Ficheiros complementares
Os ficheiro têm de ser definidos tal como previsto. Qualquer má formatação dos mesmos irá implicar erros no seu processamento. O trabalho não prevê a identificação dos erros e, assim sendo, não foi implementado nenhum algortimo para os detectar.
