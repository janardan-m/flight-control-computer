/*
 * svc_call.h
 *
 *  Created on: 30-Jan-2026
 *      Author: MedXionic
 */

#ifndef INCLUDE_SVC_CALL_H_
#define INCLUDE_SVC_CALL_H_

#ifndef SVC_CALL_H
#define SVC_CALL_H

#include <stdint.h>

/* Generic syscall entry */
uint32_t svc_call(uint32_t op,
                  uint32_t a0,
                  uint32_t a1,
                  uint32_t a2);

#endif





#endif /* INCLUDE_SVC_CALL_H_ */
