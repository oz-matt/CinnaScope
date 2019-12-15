#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#define  DEVICE_NAME "char_Driver"
#define  CLASS_NAME  "chrDev"

MODULE_LICENSE("GPL");

static int    majorNumber;
static char   message[128] = {0};
static int    numberOpens = 0;
static struct class*  charClass  = NULL;
static struct device* charDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init char_init(void){
   printk(KERN_INFO "Char_Driver: Initializing the CharDriver LKM\n");
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   printk(KERN_INFO "char_Driver: registered correctly with major number %d\n", majorNumber);
   charClass = class_create(THIS_MODULE, CLASS_NAME);
   charDevice = device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   return 0;
}

static void __exit char_exit(void){
   device_destroy(charClass, MKDEV(majorNumber, 0));
   class_unregister(charClass);
   class_destroy(charClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "Char_Driver: LKM Unloaded!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "Char_Driver: Device has been opened %d times\n", numberOpens);
   return 0;
}

ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    size_t ret = copy_to_user(buffer, message, len);
    printk(KERN_INFO "Char_Driver: Sent %zu characters to the user\n", len);
    return ret;
}

ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
    size_t ret = copy_from_user(message,buffer,len);
    printk(KERN_INFO "Char_Driver: Received %zu characters from the user\n", len);
    return ret;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Char_Driver: Device successfully closed\n");
   return 0;
}

module_init(char_init);
module_exit(char_exit);