const char* kernelSourceCode = KERNEL(
										__kernel void hellocl(__global uint *buffer)
{
    size_t gidx = get_global_id(0);
    size_t gidy = get_global_id(1);
    size_t lidx = get_local_id(0);
    buffer[gidx + 4 * gidy] = (1 << gidx) | (0x10 << gidy);
}