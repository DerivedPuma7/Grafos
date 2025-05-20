vector<Rota> construirRotas(Grafo& grafo, vector<Servico>& servicosPendentes, int capacidadeMaxima, int deposito) {
  vector<Rota> rotas;

  while (any_of(servicosPendentes.begin(), servicosPendentes.end(), [](const Servico& s){ return !s.atendido; })) {
    int cargaRestante = capacidadeMaxima;
    int noAtual = deposito;
    Rota rotaAtual;
    rotaAtual.caminho.push_back(deposito);

    while (true) {
      int melhorIndice = -1;
      int menorCusto = INT_MAX;

      // Buscar o próximo serviço compatível com a carga restante
      for (int i = 0; i < servicosPendentes.size(); i++) {
        Servico& s = servicosPendentes[i];
        if (s.atendido || s.demanda > cargaRestante) continue;

        int destino = s.tipo == NO ? s.from : s.from;
        int custoAteServico = grafo.getCaminhoMinimo(noAtual, destino);

        if (custoAteServico < menorCusto) {
          melhorIndice = i;
          menorCusto = custoAteServico;
        }
      }

      // nao há alternativas de caminhos, precisa voltar ao deposito
      if (melhorIndice == -1) {
        // Nenhum serviço possível com a carga atual → encerrar rota
        rotaAtual.custoTotal += grafo.getCaminhoMinimo(noAtual, deposito);
        rotaAtual.caminho.push_back(deposito);
        rotas.push_back(rotaAtual);
        break;
      }

      // Atender serviço
      Servico& s = servicosPendentes[melhorIndice];
      s.atendido = true;
      int destino = s.tipo == NO ? s.from : s.to;

      // Adiciona custo de ir até o serviço + custo do serviço
      rotaAtual.custoTotal += menorCusto + s.custoServico;
      rotaAtual.servicosAtendidos.push_back(s);

      // Atualiza caminho
      rotaAtual.caminho.push_back(destino);
      cargaRestante -= s.demanda;
      noAtual = destino;
    }
  }

  return rotas;
}


/**
 * TODO:
 * ler a demanda e o nó deposito de cada arquivo
 * ler todos os arquivos
 * saída de acordo com o esperado
 */