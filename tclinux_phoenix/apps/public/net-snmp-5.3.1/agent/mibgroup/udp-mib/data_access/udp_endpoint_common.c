/*
 *  UDP-MIB endpoint architecture support
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/udp-mib/data_access/udp_endpoint_common.c#1 $
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "udp-mib/udpEndpointTable/udpEndpointTable_constants.h"

#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/data_access/ipaddress.h>
#include <net-snmp/data_access/udp_endpoint.h>

#include "udp_endpoint_private.h"

/**---------------------------------------------------------------------*/
/*
 * local static vars
 */


/**---------------------------------------------------------------------*/
/*
 * initialization
 */

void
netsnmp_access_udp_endpoint_init(void)
{
}

/**---------------------------------------------------------------------*/
/*
 * container functions
 */
/**
 * initialize udp_endpoint container
 */
netsnmp_container *
netsnmp_access_udp_endpoint_container_init(u_int flags)
{
    netsnmp_container *container;

    DEBUGMSGTL(("access:udp_endpoint:container", "init\n"));

    /*
     * create the containers.
     */
    container = netsnmp_container_find("access_udp_endpoint:table_container");
    if (NULL == container)
        return NULL;

    return container;
}

/**
 * load udp_endpoint information in specified container
 *
 * @param container empty container, or NULL to have one created for you
 * @param load_flags flags to modify behaviour.
 *
 * @retval NULL  error
 * @retval !NULL pointer to container
 */
netsnmp_container*
netsnmp_access_udp_endpoint_container_load(netsnmp_container* container,
                                           u_int load_flags)
{
    int rc;

    DEBUGMSGTL(("access:udp_endpoint:container", "load\n"));

    if (NULL == container)
        container = netsnmp_access_udp_endpoint_container_init(0);
    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "no container specified/found for access_udp_endpoint\n");
        return NULL;
    }

    rc =
        netsnmp_arch_udp_endpoint_container_load(container, load_flags);
    if (0 != rc) {
        netsnmp_access_udp_endpoint_container_free(container, 0);
        container = NULL;
    }

    return container;
}

void
netsnmp_access_udp_endpoint_container_free(netsnmp_container *container,
                                           u_int free_flags)
{
    DEBUGMSGTL(("access:udp_endpoint:container", "free\n"));

    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "invalid container for netsnmp_access_udp_endpoint_free\n");
        return;
    }

    if(! (free_flags & NETSNMP_ACCESS_UDP_ENDPOINT_FREE_DONT_CLEAR)) {
        /*
         * free all items.
         */
        CONTAINER_CLEAR(container,
                        (netsnmp_container_obj_func*)free,
                        NULL);
    }

    if(! (free_flags & NETSNMP_ACCESS_UDP_ENDPOINT_FREE_KEEP_CONTAINER))
        CONTAINER_FREE(container);
}

/**---------------------------------------------------------------------*/
/*
 * entry functions
 */
/**
 */
netsnmp_udp_endpoint_entry *
netsnmp_access_udp_endpoint_entry_create(void)
{
    netsnmp_udp_endpoint_entry *entry =
        SNMP_MALLOC_TYPEDEF(netsnmp_udp_endpoint_entry);

    DEBUGMSGTL(("access:udp_endpoint:entry", "create\n"));

    if(NULL == entry)
        return NULL;

    entry->oid_index.len = 1;
    entry->oid_index.oids = (oid *) & entry->index;

    return entry;
}

/**
 */
void
netsnmp_access_udp_endpoint_entry_free(netsnmp_udp_endpoint_entry * entry)
{
    DEBUGMSGTL(("access:udp_endpoint:entry", "free\n"));

    if (NULL == entry)
        return;

    /*
     * SNMP_FREE not needed, for any of these, 
     * since the whole entry is about to be freed
     */

    free(entry);
}


/**---------------------------------------------------------------------*/
/*
 * Utility routines
 */

/**
 * \internal
 */
static void
_entry_release(netsnmp_udp_endpoint_entry * entry, void *context)
{
    netsnmp_access_udp_endpoint_entry_free(entry);
}

/**---------------------------------------------------------------------*/
/*
 *
 */
