#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

#define I2C0_NODE DT_NODELABEL(mysensor)

#define BMI160_CHIPID_REG     (0x00) // 1 byte
#define BMI160_ERR_REG        (0x02) // 1 byte
#define BMI160_PMUSTATUS_REG  (0x03) // 1 byte
#define BMI160_DATA_REG       (0x04) // 20 bytes
#define BMI160_SENSORTIME_REG (0x18) // 3 bytes

#define BMI160_INTEN_REG      (0x50) // 3 bytes

#define BMI160_DATA_LEN       (20)
#define BMI160_SENSORTIME_LEN (3) // 3 bytes


LOG_MODULE_REGISTER(Lesson6_I2C,LOG_LEVEL_DBG);

int main(void)
{
   int ret;

   // Setup
   const struct i2c_dt_spec sensor_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
   if(!device_is_ready(sensor_i2c.bus))
   {
      LOG_ERR("I2C bus %s is not ready!\r\n", sensor_i2c.bus->name);
      return -1;
   }

   uint8_t int_en[4] = 
   {
      BMI160_INTEN_REG,
      0x07, 0x70, 0x00
   };

   ret = i2c_write_dt(&sensor_i2c, int_en, sizeof(int_en));
   if(ret)
   {
      LOG_ERR("FAILED to write to I2C(0x%02x) at REG:0x%02x\r\n", sensor_i2c.addr, int_en[0]);
      return -1;
   }

   uint8_t imu_data[20] = {0};
   uint8_t sensor_rd_regs[] = {BMI160_DATA_REG, BMI160_SENSORTIME_REG};
   bool lp = false;

   // Loop - Read IMU data and print out
   while(true)
   {
      if(lp)
      {
         ret = i2c_write_read_dt(&sensor_i2c, &sensor_rd_regs[1], 1, imu_data, BMI160_SENSORTIME_LEN);
         if(ret)
         {
            LOG_ERR("FAILED write/read to I2C(0x%02x) at REG:0x%02x\r\n", sensor_i2c.addr, sensor_rd_regs[1]);
            continue;
         }

         printk("\r\nSENSORTIME: %02x%02x%02x", imu_data[0],imu_data[1], imu_data[2]);
      }
      else
      {

         ret = i2c_write_read_dt(&sensor_i2c, &sensor_rd_regs[0], 1, imu_data, BMI160_DATA_LEN);
         if(ret)
         {
            LOG_ERR("FAILED write/read to I2C(0x%02x) at REG:0x%02x\r\n", sensor_i2c.addr, sensor_rd_regs[0]);
            continue;
         }
         LOG_HEXDUMP_INF(imu_data, sizeof(imu_data),"IMU Data"); 
      }
      k_sleep(K_MSEC(5));
      lp = !lp;
   };
   return 0;
}
