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
#define DESC "my implementation of HDU OS lab's show_all_kernel_thread"
#define LICENSE "GPL"
MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESC);

#define KTHREADD_PID 2
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
        struct task_struct *parent = tsk->real_parent;
        pr_info("taskinfo: [%d] %s (%c, nice %d, parent=%d)\n", tsk->pid, name, state, task_nice(tsk), parent->pid);
    } else {
        pr_info("tsk is null\n");
    }
}
static void __find_tasks(int _pid){
    struct task_struct *tsk = get_pid_task(find_get_pid(_pid),PIDTYPE_PID);
    if(!tsk){
        return;
    }
    pr_info("-- the kthreadd --\n");
    __print_task_info(tsk);
    struct task_struct *sib;
    pr_info("-- kernel threads --\n");
    if(!list_empty(&tsk->children)){
        list_for_each_entry(sib, &tsk->children, sibling){
            __print_task_info(sib);
        }
    }
}
static int __init __aisumodule_init(void){
    pr_info("show_all_kernel_thread here\n");
    __find_tasks(KTHREADD_PID);
    return 0;
}
static void __exit __aisumodule_exit(void){
    pr_info("Leaving\n");
}
module_init(__aisumodule_init);
module_exit(__aisumodule_exit);
