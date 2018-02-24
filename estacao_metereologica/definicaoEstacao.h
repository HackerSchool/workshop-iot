/* TimeClient */
const float UTC_OFFSET = 0;

const int SEC_INTERVALO_ATUALIZACAO = 5 * 60; // Update every 5min

// flag changed in the ticker function every 10 minutes
bool AtualizacaoMetereologica = false;

String lastUpdate = "--";
