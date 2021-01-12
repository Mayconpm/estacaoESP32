#define INTERVALOMAIORENTRELEITURASMAIOR 15 * 60 //em segundos
#define INTERVALOMENORENTRELEITURASMENOR 5 * 60  //em segundos

//Watchdog
#define TEMPOWATCHDOG 300 //tempo máximo sem a chamada do reset do tempo (em segundos)

//Interface em Ethernet Shield
#define TENTATIVASSERVIDORHTTP 1 //número máximo de tentativas de conexão com o servidor HTTP

//Comunicação servidor HTTP
#define ENDERECOSERVIDOR "http://tempo.iprj.uerj.br" //endereço do servidor (String ou IP)
#define TENTATIVASDHCP 2                             //número maximo de tentativas de conexão via DHCP
#define SEGUNDOSANTESDEABORTAR 10                    //quantidade de segundos antes de abortar a conexão com o servidor HTTP (durante uma requisição)

//Armazenamento
#define EXTENSAOARQUIVOS ".txt" // deve começar com ponto

//Pluviômetro
#define VOLUMEBASCULA 0.2
#define TEMPOMINIMOENTREBASCULADAS 2000

//Termômetro e higrômetro (DHT)
#define PINODHT 5
#define DHTTYPE DHT11
//#define DHTTYPE DHT21
//#define DHTTYPE DHT22

//Comunicação XBee
#define SERVIDORXBEE "00"
#define LIVRETRANSMISSAO "FF"
#define LIMITECARACTERESPEGOS 10          //limite de caracteres pegos de cada vez
#define INTERVALOENTREENVIOS 30           //em segundos
#define TEMPOLIMITEENTREAUTORIZACOES 1800 //em segundos

//Anemômetro
#define TEMPOMINIMOENTREINTERRUPCOES 50
#define RAIODOANEMOMETRO 147 //em mm

//Alarme
#define KORSI 3.579 // constante de escorregamento
#define PINOSIRENE 9
#define DURACAOPULSO 10             // em segundos
#define INTERVALOCHECAGENSALARME 15 // em minutos
