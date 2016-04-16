
#define SPINOSEL    0
#define SPICS7280A  1
#define SPICSDAC    2
#define SPICSMEM    3

#define CS7280APOL 1 // b01

void select_device(char device);
int init_spi_master(void);

void set_SPI_pol(char pol);

void SPI_Master_write(unsigned char cData);
unsigned char SPI_Master_read(unsigned char outByte);


