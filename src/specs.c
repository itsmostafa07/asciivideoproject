#include "./specs.h"
#include "./helpers.h"

/*
 * Create a new structure for specifications of video.
 */

specs *specs_new(unsigned int frames_count, unsigned short fps, float duration, uint32_t width, uint32_t height)
{
    UNIMPLEMENTED();
}

/*
 * Serialize and convert the struct into string depend on this schema:
 *  "frames_count,fps,duration,"
 *
 * *Example:*
 *  specs* specs = specs_new(1000, 30, 3043.0, 320, 200);
 *  char* specs_as_str = specs_serialize(specs);
 *
 *  assert(strcmp(specs_as_str, "1000,30,3043.0,320,200"))
 */

char *specs_serialize(specs *specifications)
{
    UNIMPLEMENTED();
}
/*
 * Convert the string into struct depend on previous schema:
 *  "frames_count,fps,duration,width,height"
 */
specs *specs_deserialize(char *str)
{
    UNIMPLEMENTED();
}