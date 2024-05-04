#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/container_of.h>

#define AUTHOR "aisuneko <iceneko@protonmail.ch>"
#define DESC "my implementation of HDU OS lab's show_task_family"
#define LICENSE "GPL"
MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);

static int pid = 1145;
static const char task_state_array[] = {
        'R',            /* 0x00 */
        'S',            /* 0x01 */
        'D',            /* 0x02 */
        'T',            /* 0x04 */
        't',            /* 0x08 */
        'X',            /* 0x10 */
        'Z',            /* 0x20 */
        'P',            /* 0x40 */
        'I',            /* 0x80 */
};
static inline const char get_task_state(struct task_struct *tsk){
        BUILD_BUG_ON(1 + ilog2(TASK_REPORT_MAX) != ARRAY_SIZE(task_state_array));
        return task_state_array[task_state_index(tsk)];
}
static void __print_task_info(struct task_struct *tsk){
    if(tsk){
        char name[TASK_COMM_LEN];
        get_task_comm(name, tsk);
        //char *name = tsk->comm;
        char state = get_task_state(tsk);
        pr_info("taskinfo: [%d] %s: %c\n", tsk->pid, name, state);
    }
}
static void __find_tasks(int _pid){
    struct task_struct *tsk = get_pid_task(find_get_pid(_pid),PIDTYPE_PID);
    if(!tsk){
        pr_info("task not found\n");
        return;
    }
    struct task_struct *tsk_parent = tsk->real_parent;
    pr_info("-- the task --\n");
    __print_task_info(tsk);
    pr_info("-- PARENT --\n");
    __print_task_info(tsk_parent);
    struct task_struct *chi;
    pr_info("-- CHILDREN --\n");
    if(!list_empty(&tsk->children)){
        list_for_each_entry(chi, &tsk->children, sibling){
            __print_task_info(chi);
        }
    }
    struct task_struct *sib;
    pr_info("-- SIBLINGS --\n");
    if(!list_empty(&tsk_parent->children)){
        list_for_each_entry(sib, &tsk_parent->children, sibling){
            if(sib->pid != tsk->pid){
                __print_task_info(sib);
            }
        }
    }
}
static int __pidparam_set(const char *buf, const struct kernel_param *kp){
    int val;
    int ret = kstrtoint(buf, 0, &val);
    if(ret) return ret;
    __find_tasks(val);
    return param_set_int(buf, kp);
}
static const struct kernel_param_ops ops = {
    .flags = 0660,
    .set = __pidparam_set,
    .get = param_get_int
};
module_param_cb(pid, &ops, &pid, 0660);
MODULE_PARM_DESC(pid, "pid of process to get info for");
static int __init __aisumodule_init(void){
    pr_info("for info on other processes, write to that param\n");
    return 0;
}
static void __exit __aisumodule_exit(void){
    pr_info("Leaving\n");
}
module_init(__aisumodule_init);
module_exit(__aisumodule_exit);
