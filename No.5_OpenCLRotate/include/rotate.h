#ifndef __CL_ROTATE_H
#define __CL_ROTATE_H

void load_data(const char *file, uint8_t *addr, uint32_t w, uint32_t h);
void store_data(const char *file, void *addr, uint32_t w, uint32_t h);
void rotate(uint8_t *img_src, uint8_t *img_dst, int w, int h);

#endif /* __CL_ROTATE_H */
