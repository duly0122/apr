/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include "fileio.h"
#include "apr_file_io.h"
#include "apr_general.h"
#include "apr_errno.h"

static ap_filetype_e filetype_from_mode(int mode)
{
    ap_filetype_e type = APR_NOFILE;

    if (S_ISREG(mode))
        type = APR_REG;
    if (S_ISDIR(mode))
        type = APR_DIR;
    if (S_ISCHR(mode))
        type = APR_CHR;
    if (S_ISBLK(mode))
        type = APR_BLK;
    if (S_ISFIFO(mode))
        type = APR_PIPE;
    if (S_ISLNK(mode))
        type = APR_LNK;
#ifndef BEOS
    if (S_ISSOCK(mode))
        type = APR_SOCK;
#endif
    return type;
}

ap_status_t ap_getfileinfo(ap_finfo_t *finfo, ap_file_t *thefile)
{
    struct stat info;

    if (fstat(thefile->filedes, &info) == 0) {
        finfo->protection = ap_unix_mode2perms(info.st_mode);
        finfo->filetype = filetype_from_mode(info.st_mode);
        finfo->user = info.st_uid;
        finfo->group = info.st_gid;
        finfo->size = info.st_size;
        finfo->inode = info.st_ino;
        finfo->device = info.st_dev;
        ap_ansi_time_to_ap_time(&finfo->atime, info.st_atime);
        ap_ansi_time_to_ap_time(&finfo->mtime, info.st_mtime);
        ap_ansi_time_to_ap_time(&finfo->ctime, info.st_ctime);
        return APR_SUCCESS;
    }
    else {
        return errno;
    }
}

ap_status_t ap_setfileperms(const char *fname, ap_fileperms_t perms)
{
    mode_t mode = ap_unix_perms2mode(perms);

    if (chmod(fname, mode) == -1)
        return errno;
    return APR_SUCCESS;
}

ap_status_t ap_stat(ap_finfo_t *finfo, const char *fname, ap_pool_t *cont)
{
    struct stat info;

    if (stat(fname, &info) == 0) {
        finfo->protection = ap_unix_mode2perms(info.st_mode);
        finfo->filetype = filetype_from_mode(info.st_mode);
        finfo->user = info.st_uid;
        finfo->group = info.st_gid;
        finfo->size = info.st_size;
        finfo->inode = info.st_ino;
        finfo->device = info.st_dev;
        ap_ansi_time_to_ap_time(&finfo->atime, info.st_atime);
        ap_ansi_time_to_ap_time(&finfo->mtime, info.st_mtime);
        ap_ansi_time_to_ap_time(&finfo->ctime, info.st_ctime);
        return APR_SUCCESS;
    }
    else {
        return errno;
    }
}

ap_status_t ap_lstat(ap_finfo_t *finfo, const char *fname, ap_pool_t *cont)
{
    struct stat info;

    if (lstat(fname, &info) == 0) {
        finfo->protection = ap_unix_mode2perms(info.st_mode);
        finfo->filetype = filetype_from_mode(info.st_mode);
        finfo->user = info.st_uid;
        finfo->group = info.st_gid;
        finfo->size = info.st_size;
        finfo->inode = info.st_ino;
        finfo->device = info.st_dev;
        ap_ansi_time_to_ap_time(&finfo->atime, info.st_atime);
        ap_ansi_time_to_ap_time(&finfo->mtime, info.st_mtime);
        ap_ansi_time_to_ap_time(&finfo->ctime, info.st_ctime);
        return APR_SUCCESS;
    }
    else {
        return errno;
    }
}
