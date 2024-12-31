#include "dw1.hpp"

extern "C" {
void setTrainingBoost(uint16_t flag, uint16_t value, uint16_t duration) {
  if (flag == 0)
    return;

  PARTNER_PARA.trainBoostFlag = flag;
  PARTNER_PARA.trainBoostValue = value;
  PARTNER_PARA.trainBoostTimer = duration * 1200;
}
}