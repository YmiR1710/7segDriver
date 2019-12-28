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

int set_number(int num){
	if (num == 0){
		gpio_set_value(5, 1);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 1);
		gpio_set_value(12, 1);
		gpio_set_value(16, 1);
		gpio_set_value(25, 0);
	}
	else if (num == 1){
		gpio_set_value(5, 0);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 0);
		gpio_set_value(12, 0);
		gpio_set_value(16, 0);
		gpio_set_value(25, 0);
	}
	else if (num == 2){
		gpio_set_value(5, 1);
		gpio_set_value(6, 1);
		gpio_set_value(13, 0);
		gpio_set_value(26, 1);
		gpio_set_value(12, 1);
		gpio_set_value(16, 0);
		gpio_set_value(25, 1);
	}
	else if (num == 3){
		gpio_set_value(5, 1);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 1);
		gpio_set_value(12, 0);
		gpio_set_value(16, 0);
		gpio_set_value(25, 1);
	}
	else if (num == 4){
		gpio_set_value(5, 0);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 0);
		gpio_set_value(12, 0);
		gpio_set_value(16, 1);
		gpio_set_value(25, 1);
	}
	else if (num == 5){
		gpio_set_value(5, 1);
		gpio_set_value(6, 0);
		gpio_set_value(13, 1);
		gpio_set_value(26, 1);
		gpio_set_value(12, 0);
		gpio_set_value(16, 1);
		gpio_set_value(25, 1);
	}
	else if (num == 6){
		gpio_set_value(5, 1);
		gpio_set_value(6, 0);
		gpio_set_value(13, 1);
		gpio_set_value(26, 1);
		gpio_set_value(12, 1);
		gpio_set_value(16, 1);
		gpio_set_value(25, 1);
	}
	else if (num == 7){
		gpio_set_value(5, 1);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 0);
		gpio_set_value(12, 0);
		gpio_set_value(16, 0);
		gpio_set_value(25, 0);
	}
	else if (num == 8){
		gpio_set_value(5, 1);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 1);
		gpio_set_value(12, 1);
		gpio_set_value(16, 1);
		gpio_set_value(25, 1);
	}
	else if (num == 9){
		gpio_set_value(5, 1);
		gpio_set_value(6, 1);
		gpio_set_value(13, 1);
		gpio_set_value(26, 1);
		gpio_set_value(12, 0);
		gpio_set_value(16, 1);
		gpio_set_value(25, 1);
	}
	return 0;
}

static ssize_t mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
        return sprintf(buf, "%i\n", number);
}

static ssize_t mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    int i;
    for (i = 0; i < count; i++){
		number = buf[0] - '0';
		set_number(number);
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
	set_number(number);
	return 0;
}


static void __exit ebbLED_exit(void){
	kobject_put(ebb_kobj);
	printk(KERN_INFO "Work finished\n");
}


module_init(ebbLED_init);
module_exit(ebbLED_exit);
