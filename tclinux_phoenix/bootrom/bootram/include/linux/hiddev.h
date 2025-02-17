#ifndef _HIDDEV_H
#define _HIDDEV_H

/*
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/bootrom/bootram/include/linux/hiddev.h#1 $
 *
 *  Copyright (c) 1999-2000 Vojtech Pavlik
 *
 *  Sponsored by SuSE
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * 
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@suse.cz>, or by paper mail:
 * Vojtech Pavlik, Ucitelska 1576, Prague 8, 182 00 Czech Republic
 */

/*
 * The event structure itself
 */

struct hiddev_event {
	unsigned hid;
	signed int value;
};

struct hiddev_devinfo {
	unsigned int bustype;
	unsigned int busnum;
	unsigned int devnum;
	unsigned int ifnum;
	short vendor;
	short product;
	short version;
	unsigned num_applications;
};

#define HID_STRING_SIZE 256
struct hiddev_string_descriptor {
	int index;
	char value[HID_STRING_SIZE];
};

struct hiddev_report_info {
	unsigned report_type;
	unsigned report_id;
	unsigned num_fields;
};

/* To do a GUSAGE/SUSAGE, fill in at least usage_code,  report_type and 
 * report_id.  Set report_id to REPORT_ID_UNKNOWN if the rest of the fields 
 * are unknown.  Otherwise use a usage_ref struct filled in from a previous 
 * successful GUSAGE/SUSAGE call to save time.  To actually send a value
 * to the device, perform a SUSAGE first, followed by a SREPORT.  If an
 * INITREPORT is done, a GREPORT isn't necessary before a GUSAGE.
 */
#define HID_REPORT_ID_UNKNOWN 0xffffffff
#define HID_REPORT_ID_FIRST   0x00000100
#define HID_REPORT_ID_NEXT    0x00000200
#define HID_REPORT_ID_MASK    0x000000ff
#define HID_REPORT_ID_MAX     0x000000ff

#define HID_REPORT_TYPE_INPUT	1
#define HID_REPORT_TYPE_OUTPUT	2
#define HID_REPORT_TYPE_FEATURE	3
#define HID_REPORT_TYPE_MIN     1
#define HID_REPORT_TYPE_MAX     3

struct hiddev_field_info {
	unsigned report_type;
	unsigned report_id;
	unsigned field_index;
	unsigned maxusage;
	unsigned flags;
	unsigned physical;		/* physical usage for this field */
	unsigned logical;		/* logical usage for this field */
	unsigned application;		/* application usage for this field */
	__s32 logical_minimum;
	__s32 logical_maximum;
	__s32 physical_minimum;
	__s32 physical_maximum;
	unsigned unit_exponent;
	unsigned unit;
};

/* Fill in report_type, report_id and field_index to get the information on a
 * field.
 */
#define HID_FIELD_CONSTANT		0x001
#define HID_FIELD_VARIABLE		0x002
#define HID_FIELD_RELATIVE		0x004
#define HID_FIELD_WRAP			0x008	
#define HID_FIELD_NONLINEAR		0x010
#define HID_FIELD_NO_PREFERRED		0x020
#define HID_FIELD_NULL_STATE		0x040
#define HID_FIELD_VOLATILE		0x080
#define HID_FIELD_BUFFERED_BYTE		0x100

struct hiddev_usage_ref {
	unsigned report_type;
	unsigned report_id;
	unsigned field_index;
	unsigned usage_index;
	unsigned usage_code;
	__s32 value;
};


/*
 * Protocol version.
 */

#define HID_VERSION		0x010002

/*
 * IOCTLs (0x00 - 0x7f)
 */

#define HIDIOCGVERSION		_IOR('H', 0x01, int)
#define HIDIOCAPPLICATION	_IO('H', 0x02)
#define HIDIOCGDEVINFO		_IOR('H', 0x03, struct hiddev_devinfo)
#define HIDIOCGSTRING		_IOR('H', 0x04, struct hiddev_string_descriptor)
#define HIDIOCINITREPORT	_IO('H', 0x05)
#define HIDIOCGNAME(len)	_IOC(_IOC_READ, 'H', 0x06, len)
#define HIDIOCGREPORT		_IOW('H', 0x07, struct hiddev_report_info)
#define HIDIOCSREPORT		_IOW('H', 0x08, struct hiddev_report_info)
#define HIDIOCGREPORTINFO       _IOWR('H', 0x09, struct hiddev_report_info)
#define HIDIOCGFIELDINFO        _IOWR('H', 0x0A, struct hiddev_field_info)
#define HIDIOCGUSAGE            _IOWR('H', 0x0B, struct hiddev_usage_ref)
#define HIDIOCSUSAGE            _IOW('H', 0x0C, struct hiddev_usage_ref)
#define HIDIOCGUCODE            _IOWR('H', 0x0D, struct hiddev_usage_ref)

/* To traverse the input report descriptor info for a HID device, perform the 
 * following:
 *
 *  rinfo.report_type = HID_REPORT_TYPE_INPUT;
 *  rinfo.report_id = HID_REPORT_ID_FIRST;
 *  ret = ioctl(fd, HIDIOCGREPORTINFO, &rinfo);
 *
 *  while (ret >= 0) {
 *      for (i = 0; i < rinfo.num_fields; i++) { 
 *	    finfo.report_type = rinfo.report_type;
 *          finfo.report_id = rinfo.report_id;
 *          finfo.field_index = i;
 *          ioctl(fd, HIDIOCGFIELDINFO, &finfo);
 *          for (j = 0; j < finfo.maxusage; j++) {
 *              uref.field_index = i;
 *		uref.usage_index = j;
 *		ioctl(fd, HIDIOCGUCODE, &uref);
 *		ioctl(fd, HIDIOCGUSAGE, &uref);
 *          }
 *	}
 *	uref.report_id |= HID_REPORT_ID_NEXT;
 *	ret = ioctl(fd, HIDIOCGREPORTINFO, &uref);
 *  }
 */


#ifdef __KERNEL__

/*
 * In-kernel definitions.
 */

#ifdef CONFIG_USB_HIDDEV
int hiddev_connect(struct hid_device *);
void hiddev_disconnect(struct hid_device *);
void hiddev_hid_event(struct hid_device *, unsigned int usage, int value);
int __init hiddev_init(void);
void __exit hiddev_exit(void);
#else
static inline void *hiddev_connect(struct hid_device *hid) { return NULL; }
static inline void hiddev_disconnect(struct hid_device *hid) { }
static inline void hiddev_event(struct hid_device *hid, unsigned int usage, int value) { }
static inline int hiddev_init(void) { return 0; }
static inline void hiddev_exit(void) { }
#endif

#endif
#endif
