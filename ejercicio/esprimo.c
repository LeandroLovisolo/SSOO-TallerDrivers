#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "esprimo"

struct esprimo_dev {
    int number;
};
struct esprimo_dev esprimo_dev;

int my_atoi(const char *p, size_t size) {
    int k = 0, i = 0;
    for(i = 0; i < size; i++) {
        k = (k<<3)+(k<<1)+(*p)-'0';
        p++;
    }
    return k;
}

ssize_t esprimo_read(struct file * _file, char __user * user, size_t size, loff_t * offset) {
    int primo = 1, i;

    printk("number = %d\n", esprimo_dev.number);

    for(i = 2; i < esprimo_dev.number; i++) {
        if(esprimo_dev.number % i == 0) {
            primo = 0;
            break;
        }
    }

    user[0] = primo ? '1' : '0';
    printk(KERN_ALERT "esprimo: devolvimos %c\n", user[0]);
    return 1;
}

ssize_t esprimo_write(struct file * _file, const char __user * user, size_t size, loff_t * offset) {
    esprimo_dev.number = my_atoi(user, size - 1);
    printk(KERN_ALERT "esprimo: leimos numero %d\n", esprimo_dev.number);
    return size;
}

// file_operations que se asociara al cdev
static struct file_operations esprimo_fops = {
    .owner = THIS_MODULE,
    .read = esprimo_read,
    .write = esprimo_write
};

struct miscdevice esprimo_miscdev = {
    MISC_DYNAMIC_MINOR,
    DEVICE_NAME,
    &esprimo_fops,
};

static int __init esprimo_init(void) {
    if (misc_register(&esprimo_miscdev)) {
        printk(KERN_ALERT "esprimo: Error al registrar misc device\n");
        return 1;
    }
    esprimo_dev.number = 0;

    printk(KERN_ALERT "esprimo: inicializado\n");
    return 0;
}

static void __exit esprimo_exit(void) {
    misc_deregister(&esprimo_miscdev);
}

module_init(esprimo_init);
module_exit(esprimo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Docentes de Sistemas Operativos");
MODULE_DESCRIPTION("Decide si un numero es primo");
