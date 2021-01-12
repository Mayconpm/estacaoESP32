//Estação
#define INTERVALOENTRELEITURAS 3 //em minutos
#define SERVIDORNTP "br.pool.ntp.org"
#define CHAVEDECOMUNICACAO "05" //chave de identificação única com o servidor
#define TIMEZONE "BRST+3"
#define SSIDWIFI "SSID"
#define PASSWORDWIFI "SENHA"

//Comunicação servidor HTTP
#define ENDERECOSERVIDOR "Servidor" //endereço do servidor (String ou IP)
#define TENTATIVASSERVIDORHTTP 1    //número máximo de tentativas de conexão com o servidor HTTP
#define TENTATIVASDHCP 2            //número maximo de tentativas de conexão via DHCP
#define SEGUNDOSANTESDEABORTAR 10   //quantidade de segundos antes de abortar a conexão com o servidor HTTP (durante uma requisição)

//Armazenamento
#define DIRETORIODADOS "/"                 //deve começar e terminar com barras
#define EXTENSAOARQUIVOS ".txt"            // deve começar com ponto
#define EXTENSAOARQUIVOSTEMPORARIOS ".tmp" // deve começar com ponto

//Pluviômetro
#define VOLUMEBASCULA 0.2
#define TEMPOMINIMOENTREBASCULADAS 2000
#define PINOPLUVIOMETRO 18
