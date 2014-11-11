
typedef struct
{
  int length;
  uint8_t operand_count;
  uint8_t reserved[3];
  uint32_t sum;
  uint16_t operands[32763];
  uint8_t padding;
} calc_message_t;