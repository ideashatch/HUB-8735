#ifndef LineNotify_h
#define LineNotify_h

#include "Print.h"
#include "Client.h"
#include "IPAddress.h"
#include "ssl_drv.h"
#include "VideoStream.h"

struct mbedtls_ssl_context;

char *print_utf8_zh_code(char *words);

class LineNotify : public Client {

    public:
        LineNotify();
        LineNotify(uint8_t sock);

        uint8_t status();
        virtual int connect(IPAddress ip, uint16_t port);
        virtual int connect(const char *host, uint16_t port);
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buf, size_t size);
        virtual int available();
        virtual int read();
        virtual int read(uint8_t *buf, size_t size);
        virtual int peek();
        virtual void flush();
        virtual void stop();
        virtual uint8_t connected();
        virtual operator bool();

        void setRootCA(unsigned char *rootCA);
        void setClientCertificate(unsigned char *client_ca, unsigned char *private_key);
        void setPreSharedKey(unsigned char *pskIdent, unsigned char *psKey);  // psKey expressed as hexadecimal string
		void setLineToken(char *charLineToken);

        int connect(const char *host, uint16_t port, unsigned char* rootCABuff, unsigned char* cli_cert, unsigned char* cli_key);
        int connect(IPAddress ip, uint16_t port, unsigned char* rootCABuff, unsigned char* cli_cert, unsigned char* cli_key);
        int connect(const char *host, uint16_t port, unsigned char* pskIdent, unsigned char* psKey);
        int connect(IPAddress ip, uint16_t port, unsigned char* pskIdent, unsigned char* psKey);

		void send(char *msg, bool notificationDisabled);
		void sendCameraImage(String msg, LineNotify client, Video camera, int ch, bool notificationDisabled);
		void sendurlImage(char *msg, char *imageFullsize, char *imageThumbnail, bool notificationDisabled);
		void sendSDImage(String msg, LineNotify client, String imagePath, bool notificationDisabled);
		void sendStricker(char *msg, int stickerId, int stickerPackageId, bool notificationDisabled);
        using Print::write;
        int setRecvTimeout(int timeout);

    private:
        int _sock;
        bool _is_connected;
        sslclient_context sslclient;
        SSLDrv ssldrv;

		String _LineToken;
		char *_messagestr;
        unsigned char *_rootCABuff;
        unsigned char *_cli_cert;
        unsigned char *_cli_key;
        unsigned char *_psKey;
        unsigned char *_pskIdent;
        char *_sni_hostname;
        String param_data;
};
//static const char line_notify_str_0[] PROGMEM = "POST /api/notify HTTP/1.1";
//static const char line_notify_str_1[] PROGMEM = "\r\n";
//static const char line_notify_str_2[] PROGMEM = "Host: ";
//static const char line_notify_str_3[] PROGMEM = "notify-api.line.me";	//host
//static const char line_notify_str_4[] PROGMEM = "Authorization: Bearer ";
//static const char line_notify_str_5[] PROGMEM = "Content-Length: ";
//static const char line_notify_str_6[] PROGMEM = "Content-Type: application/x-www-form-urlencoded\r\n\r\n";



static const char line_notify_test_url_0[] PROGMEM = "https://upload.wikimedia.org/wikipedia/commons/thumb/3/3a/Cat03.jpg/240px-Cat03.jpg";

static const char line_notify_str_1[] PROGMEM = "notify-api.line.me";
static const char line_notify_str_2[] PROGMEM = " ";
static const char line_notify_str_3[] PROGMEM = "HUB 8735";
static const char line_notify_str_4[] PROGMEM = "\r\n";
static const char line_notify_str_5[] PROGMEM = "POST /api/notify HTTP/1.1";
static const char line_notify_str_6[] PROGMEM = "cache-control: no-cache";
static const char line_notify_str_7[] PROGMEM = "Authorization: Bearer ";
static const char line_notify_str_8[] PROGMEM = "Content-Type: multipart/form-data; boundary=";
static const char line_notify_str_9[] PROGMEM = "User-Agent: ";
static const char line_notify_str_10[] PROGMEM = "Accept: */*";
static const char line_notify_str_11[] PROGMEM = "Host: ";
static const char line_notify_str_12[] PROGMEM = "accept-encoding: gzip, deflate";
static const char line_notify_str_13[] PROGMEM = "Connection: close";
static const char line_notify_str_14[] PROGMEM = "content-length: ";
static const char line_notify_str_15[] PROGMEM = "/api/notify";
static const char line_notify_str_16[] PROGMEM = "Content-Disposition: form-data; name=\"imageFile\"; filename=\"";
static const char line_notify_str_17[] PROGMEM = "Content-Type: ";
static const char line_notify_str_18[] PROGMEM = "Content-Disposition: form-data; name=\"";
static const char line_notify_str_19[] PROGMEM = "message";
static const char line_notify_str_20[] PROGMEM = "stickerPackageId";
static const char line_notify_str_21[] PROGMEM = "stickerId";
static const char line_notify_str_22[] PROGMEM = "imageThumbnail";
static const char line_notify_str_23[] PROGMEM = "imageFullsize";
static const char line_notify_str_24[] PROGMEM = "--";
static const char line_notify_str_25[] PROGMEM = "\"";
static const char line_notify_str_26[] PROGMEM = ".jpg";
static const char line_notify_str_27[] PROGMEM = ".jpeg";
static const char line_notify_str_28[] PROGMEM = "image/jpeg";
static const char line_notify_str_29[] PROGMEM = ".gif";
static const char line_notify_str_30[] PROGMEM = "image/gif";
static const char line_notify_str_31[] PROGMEM = ".png";
static const char line_notify_str_32[] PROGMEM = "image/png";
static const char line_notify_str_33[] PROGMEM = ".bmp";
static const char line_notify_str_34[] PROGMEM = "image/bmp";
static const char line_notify_str_35[] PROGMEM = ".";
static const char line_notify_str_36[] PROGMEM = "HTTP/1.1 ";
static const char line_notify_str_37[] PROGMEM = " ";
static const char line_notify_str_38[] PROGMEM = "X-RateLimit-Limit: ";
static const char line_notify_str_39[] PROGMEM = "X-RateLimit-ImageLimit: ";
static const char line_notify_str_40[] PROGMEM = "X-RateLimit-Remaining: ";
static const char line_notify_str_41[] PROGMEM = "X-RateLimit-ImageRemaining: ";
static const char line_notify_str_42[] PROGMEM = "X-RateLimit-Reset: ";
static const char line_notify_str_43[] PROGMEM = "/esp";
static const char line_notify_str_44[] PROGMEM = "read timed out";
static const char line_notify_str_45[] PROGMEM = "status";
static const char line_notify_str_46[] PROGMEM = "Connection: ";
static const char line_notify_str_47[] PROGMEM = "Content-Length: ";
static const char line_notify_str_48[] PROGMEM = ";";
static const char line_notify_str_49[] PROGMEM = "Location: ";
static const char line_notify_str_50[] PROGMEM = "Transfer-Encoding: ";
static const char line_notify_str_51[] PROGMEM = "chunked";
static const char line_notify_str_52[] PROGMEM = "pool.ntp.org";
static const char line_notify_str_53[] PROGMEM = "time.nist.gov";
static const char line_notify_str_54[] PROGMEM = "=_abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char line_notify_str_55[] PROGMEM = "File I/O error";
static const char line_notify_str_56[] PROGMEM = "connection closed";
static const char line_notify_str_57[] PROGMEM = "notificationDisabled";
static const char line_notify_str_58[] PROGMEM = "true";
static const char line_notify_str_59[] PROGMEM = "false";
static const char line_notify_str_60[] PROGMEM = "https://maps.googleapis.com/maps/api/staticmap?key=";
static const char line_notify_str_61[] PROGMEM = "https://www.google.com/maps/@?api=1&map_action=map&center=";
static const char line_notify_str_62[] PROGMEM = "&center=";
static const char line_notify_str_63[] PROGMEM = "&zoom=";
static const char line_notify_str_64[] PROGMEM = "&maptype=";
static const char line_notify_str_65[] PROGMEM = "&size=";
static const char line_notify_str_66[] PROGMEM = "&markers=";
static const char line_notify_str_67[] PROGMEM = "&basemap=";
static const char line_notify_str_68[] PROGMEM = "\n\n";
static const char line_notify_str_69[] PROGMEM = "application/x-www-form-urlencoded";
static const char line_notify_str_70[] PROGMEM = "=";
static const char line_notify_str_71[] PROGMEM = "&";
static const char line_notify_bundary[] PROGMEM = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
#endif
