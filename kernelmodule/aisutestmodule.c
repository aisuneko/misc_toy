#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/string.h>

#define AUTHOR "aisuneko <iceneko@protonmail.ch>"
#define DESC "demo of object creation under /sys"
#define LICENSE "GPL"
MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);

#define NAME(var) #var
static char *aisumodule_str = "placeholder";
//module_param(aisumodule_str, charp, 0660);

static int __aisuparam_get(char *buffer, const struct kernel_param *kp){
    pr_info("%s (this time from __aisuparam_get)\n", buffer);
    return param_get_charp(buffer, kp);
}
static const struct kernel_param_ops ops = {
    .flags = 0660,
    .set = param_set_charp,
    .get = __aisuparam_get,
};
module_param_cb(aisumodule_str, &ops, &aisumodule_str, 0660);
MODULE_PARM_DESC(aisumodule_str, "write ur arbitrary message here");

static ssize_t __aisumodule_showattr(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    return sprintf(buf, "%s\n", aisumodule_str);
}
static ssize_t __aisumodule_storeattr(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    return sscanf(buf, "%s", aisumodule_str);
}
static struct kobject *aisumodule;
static struct kobj_attribute aisuattr = __ATTR(aisumodule_str, 0660, __aisumodule_showattr, __aisumodule_storeattr);

static int __init __aisumodule_init(void){
    pr_info("Hi there, here's your message: %s\n", aisumodule_str);
    aisumodule = kobject_create_and_add("aisumodule", NULL);
    int err = (aisumodule == NULL);
    if(!err){
        pr_info("/sys/aisumodule folder created\n");
        err = sysfs_create_file(aisumodule, &aisuattr.attr);
        if(!err){
            pr_info("message added to /sys/aisumodule/%s\n", NAME(aisumodule_str));
        } else {
            pr_info("err creating file");
        }
    } else {
        pr_info("err creating kobject");
    }
    return err;
}
static void __exit __aisumodule_exit(void){
    pr_info("Leaving, btw I'll remove the object for you\n");
    sysfs_remove_file(aisumodule, &aisuattr.attr);
    kobject_put(aisumodule);
}

module_init(__aisumodule_init);
module_exit(__aisumodule_exit);

