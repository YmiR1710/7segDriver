#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vitaliy Volianskyi");
MODULE_DESCRIPTION("A simple Linux driver for 7seg");
MODULE_VERSION("0.1");

static unsigned int number = 0;
module_param(number, uint, 0660);
MODULE_PARM_DESC(number, "Number on segdisplay (default=0)");


static unsigned int gpios[] = {5, 6, 13, 26, 12, 16, 25};

char digits[] = {0b11111100,0b01100000,0b11011010,0b11110010,0b01100110,
        0b10110110,0b10111110,0b11100000,0b11111110,0b11110110};

void set_number(char num){
        int i = 0;
        while(i<7){
                if(num & 1){
                        gpio_set_value(gpios[i], 1);
        }
                else{
                        gpio_set_value(gpios[i], 0);
        }
        num >>= 1;
        i++;
    }
}

static ssize_t mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
        return sprintf(buf, "%i\n", number);
}


static ssize_t mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    if (buf[0] >= '0' && buf[0] <= '9') {
        number = buf[0] - '0';
        set_number(digits[number]);
    } else {
        printk(KERN_ERR "Wrong number\n");
    }
    return count;
}

static struct kobj_attribute mode_attr = __ATTR(number, 0660, mode_show, mode_store);

static struct attribute *ebb_attrs[] = {
   &mode_attr.attr,
   NULL,
};

static struct attribute_group attr_group = {
   .name  = "segdisplay",
   .attrs = ebb_attrs,
};

static struct kobject *ebb_kobj;


static int __init ebbLED_init(void){
    int result = 0;
    printk(KERN_INFO "Work started\n");
    ebb_kobj = kobject_create_and_add("segdisplay", kernel_kobj->parent);
    if(!ebb_kobj){
        return -ENOMEM;
    }
    result = sysfs_create_group(ebb_kobj, &attr_group);
    if(result) {
        kobject_put(ebb_kobj);
        return result;
    }
        set_number(digits[number]);
        return 0;
}


static void __exit ebbLED_exit(void){
   kobject_put(ebb_kobj);
   printk(KERN_INFO "Work finished\n");
}


module_init(ebbLED_init);
module_exit(ebbLED_exit);