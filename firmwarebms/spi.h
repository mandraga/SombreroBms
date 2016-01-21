
#define NOSEL    0
#define CS7280A  1
#define CSDAC    2
#define CSMEM    3

#define CS7280APOL 3 // b11
#define CS7280APOL 0 // b00

void select_device(char device);
int init_spi_master(void);

void set_SPI_pol(char pol);

void SPI_Master_write(unsigned char cData);
unsigned char SPI_Master_read();


