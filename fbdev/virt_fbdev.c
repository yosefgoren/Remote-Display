#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fb.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define FB_WIDTH 24
#define FB_HEIGHT 20
#define FB_BPP 32  // bits per pixel
#define FB_NAME "fb1"

struct fb_info *info;
static char *fb_data = NULL;  // Pointer to framebuffer data

static struct fb_var_screeninfo virtual_fb_var = {
    .xres = FB_WIDTH,
    .yres = FB_HEIGHT,
    .xres_virtual = FB_WIDTH,
    .yres_virtual = FB_HEIGHT,
    .bits_per_pixel = FB_BPP,
    .red = {16, 8, 0},
    .green = {8, 8, 0},
    .blue = {0, 8, 0},
    .transp = {24, 8, 0},
    .activate = FB_ACTIVATE_NOW,
};

static struct fb_fix_screeninfo virtual_fb_fix = {
    .id = FB_NAME,
    .type = FB_TYPE_PACKED_PIXELS,
    .visual = FB_VISUAL_TRUECOLOR,
    .line_length = FB_WIDTH * (FB_BPP / 8),
    .accel = FB_ACCEL_NONE,
};

static ssize_t fb_read(struct fb_info *info, char __user *buf, size_t count, loff_t *ppos)
{
    if (*ppos >= FB_WIDTH * FB_HEIGHT * (FB_BPP / 8))
        return 0;

    if (*ppos + count > FB_WIDTH * FB_HEIGHT * (FB_BPP / 8))
        count = FB_WIDTH * FB_HEIGHT * (FB_BPP / 8) - *ppos;

    if (copy_to_user(buf, fb_data + *ppos, count))
        return -EFAULT;

    *ppos += count;
    return count;
}

static ssize_t fb_write(struct fb_info *info, const char __user *buf, size_t count, loff_t *ppos)
{
    if (*ppos >= FB_WIDTH * FB_HEIGHT * (FB_BPP / 8))
        return -ENOSPC;

    if (*ppos + count > FB_WIDTH * FB_HEIGHT * (FB_BPP / 8))
        count = FB_WIDTH * FB_HEIGHT * (FB_BPP / 8) - *ppos;

    if (copy_from_user(fb_data + *ppos, buf, count))
        return -EFAULT;

    *ppos += count;
    return count;
}

static struct fb_ops virtual_fb_ops = {
    .owner = THIS_MODULE,
    .fb_read = fb_read,
    .fb_write = fb_write,
    .fb_fillrect = NULL,
    .fb_copyarea = NULL,
    .fb_imageblit = NULL,
};

static int __init fbdev_init(void)
{
    int retval = 0;

    // Allocate memory for framebuffer
    fb_data = vzalloc(FB_WIDTH * FB_HEIGHT * (FB_BPP / 8));
    if (!fb_data)
        return -ENOMEM;

    // Allocate framebuffer info structure
    info = framebuffer_alloc(0, NULL);
    if (!info) {
        vfree(fb_data);
        return -ENOMEM;
    }

    // Set framebuffer information
    info->screen_base = fb_data;
    info->fbops = &virtual_fb_ops;
    info->fix = virtual_fb_fix;
    info->var = virtual_fb_var;
    info->screen_size = FB_WIDTH * FB_HEIGHT * (FB_BPP / 8);

    // Register framebuffer device
    retval = register_framebuffer(info);
    if (retval < 0) {
        framebuffer_release(info);
        vfree(fb_data);
        return retval;
    }

    printk(KERN_INFO "fbdev: framebuffer device registered as /dev/fb1\n");
    return 0;
}

static void __exit fbdev_exit(void)
{
    unregister_framebuffer(info);
    framebuffer_release(info);
    vfree(fb_data);
    printk(KERN_INFO "fbdev: framebuffer device unregistered\n");
}

module_init(fbdev_init);
module_exit(fbdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Minimal fbdev driver");
