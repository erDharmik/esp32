#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "string.h"
#include "esp_spiffs.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "nvs.h"


const char *TAG = "SPIFFS"; 


typedef struct _wifi
{
    char ssid[10];
    char pass[10];
}wifi;

typedef struct _wifiData
{
    char ssid[10];
    char pass[10];
}wifiData;

wifi cred;
wifiData provCred;



esp_err_t init_file_system()
{
	esp_err_t ret = ESP_FAIL;

	esp_vfs_spiffs_conf_t conf = {
			.base_path = "/spiffs",
			.partition_label = NULL,
			.max_files = 15,
			.format_if_mount_failed = false
	};

	ret = esp_vfs_spiffs_register(&conf);

	if(ret != ESP_OK)
	{
		if (ret == ESP_FAIL)
		{
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		}
		else if (ret == ESP_ERR_NOT_FOUND)
		{
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		}
		else
		{
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
		}
	}

     size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }


	return ret;


}

void updateData()
{
    
  FILE *fp;

    fp = fopen("/spiffs/prov", "wb");
    fwrite(&cred, sizeof(wifi), 1, fp);
    ESP_LOGI(TAG, "File written");
    fclose(fp);
}

esp_err_t readData(){

    FILE *fp;
    fp = fopen("/spiffs/prov","rb");
    fread(&provCred,sizeof(wifiData),1,fp);
    printf("SSID: [%s]\n", provCred.ssid);
    fclose(fp);
	// printf("PASS: [%s]\n", cred.pass);
	// printf("%s\n", buffer);

    return ESP_OK;

}

void clearData()
{
	memset((void *)&cred, 0, sizeof(wifi));
	updateData();
}

void app_main(void)
{


    strcpy(cred.ssid, "Pint_705");
    strcpy(cred.pass, "Pint1705");

    printf("SSID --> %s\n", cred.ssid);
    printf("PAssword --> %s\n", cred.pass);

    ESP_ERROR_CHECK(init_file_system());
	ESP_LOGI(TAG, "Initialized file system successfully");

    updateData();
    readData();

    // readData();
   
 
   
}
