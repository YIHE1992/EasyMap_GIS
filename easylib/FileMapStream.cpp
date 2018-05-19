#include "FileMapStream.h"

#include <stdio.h>

namespace easymap
{

const dword FILEMAPMININCREASSIZE = 256;
const dword FILEMAPMAXINCREASSIZE = 262144;
const dword MAPFILEINCREASSIZE = 16384;

CFileMapStream::CFileMapStream(const char* pcFileName,
    const bool readonly)
{
    INIT_REFCOUNT

    m_MapFileName[0]        = 0;
    m_ReadOnly              = readonly;
    m_MapFileIncreasSize    = MAPFILEINCREASSIZE;
    m_hMapFile              = INVALID_HANDLE_VALUE;
    m_hMappingObject        = NULL;
    m_pViewBase             = NULL;

    this->Initial(pcFileName, readonly);
}

CFileMapStream::CFileMapStream()
{
    INIT_REFCOUNT

    m_MapFileName[0]        = 0;
    m_MapFileIncreasSize    = MAPFILEINCREASSIZE;
    m_hMapFile              = INVALID_HANDLE_VALUE;
    m_hMappingObject        = NULL;
    m_pViewBase             = NULL;
}

CFileMapStream::~CFileMapStream()
{
    this->Unload();
}

bool __stdcall CFileMapStream::GotoInterface(const char* const interfacename, void** pp)
{
    if (_invalid(pp)) return false;
    assert(!*pp);

    if ((0 == strcmp(interfacename, "IObj"))
        || (0 == strcmp(interfacename, "IStreamX"))
        || (0 == strcmp(interfacename, "CStream"))
        || (0 == strcmp(interfacename, "CFileStream"))
        || (0 == strcmp(interfacename, "CFileMapStream")))
    {
        *pp = this;
    }
    else
    {
        *pp = NULL;
        return false;
    }

    static_cast<IObj*>(*pp)->_AddRef();
    return true;
}

bool __stdcall CFileMapStream::Clone(IObj** ppObj) const
{
    if (_invalid(ppObj)) return false;
//    if (_valid(*ppObj)) (*ppObj)->_Release();
    assert(!*ppObj);

    IObjPtr po;
    this->Clone(po);
    (*ppObj) = po._p();
    (*ppObj)->_AddRef();
    return true;
}

bool CFileMapStream::Clone(IObjPtr& pObj) const
{
    pObj.Clear();
    CFileMapStreamPtr pNewStream = new CFileMapStream(m_MapFileName, m_ReadOnly);
    if (0 == pNewStream->m_MapFileName[0])
    {
        return false;
    }
    else
    {
        pObj = pNewStream._p();
        return true;
    }
}

void CFileMapStream::Initial(const char* pcFileName,
    const bool readonly)
{
    this->Unload();

    m_ReadOnly = readonly;

    //����CreateFile()��һ���ļ�
    dword DesiredAccess;
    dword ShareMode;
    dword CreationDisposition;
    dword FlagsAndAttributes;
    if (m_ReadOnly)
    {
        DesiredAccess       = GENERIC_READ;
        ShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE;
        CreationDisposition = OPEN_EXISTING;
        FlagsAndAttributes  = FILE_ATTRIBUTE_READONLY | FILE_FLAG_RANDOM_ACCESS;
    }
    else
    {
        DesiredAccess       = GENERIC_READ | GENERIC_WRITE;
        ShareMode           = FILE_SHARE_READ;// | FILE_SHARE_WRITE;
        CreationDisposition = OPEN_ALWAYS;
        FlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS;
    }

    m_hMapFile = ::CreateFile(pcFileName, DesiredAccess, ShareMode,
        NULL, CreationDisposition, FlagsAndAttributes, NULL);
    if (_invalid(m_hMapFile) || (INVALID_HANDLE_VALUE == m_hMapFile))
    {
        this->WhenShitHappen();
        return;
    }

    //�ļ�Ҫô���´����ģ�Ҫôsize >= FILEMAPHEADERLENGTH

    //m_MapFileSize��ʼ����ӳ���ļ���ǰ��С
    m_MapFileSize = ::GetFileSize(m_hMapFile, NULL);

    //------------- �����ļ������Ƿ���ȷ -------------
    //�����½����ļ�����עxxB
    bool xxA = 0 < m_MapFileSize;
    //�ļ����Ȳ����ܶ����ļ�ͷ
    bool xxB = FILEMAPHEADERLENGTH > m_MapFileSize;
    //ֻ����ʱ�����½��ļ�
    bool xxC = (0 == m_MapFileSize) && m_ReadOnly;
    if ((xxA&&xxB)||xxC)
    {
        ::CloseHandle(m_hMapFile);
        this->WhenShitHappen();
        return;
    }
    //------------------------------------------------

    if (0 == m_MapFileSize)
    {
        //�½����ļ�����д��FileStreamHeader
        FileStreamHeader header;
        ::memset(&header, 0, sizeof(FileStreamHeader));
        ::strcpy((char*)header.Description, FILEMAPSTREAMIDENTIFY);
        ::strcat((char*)header.Description, "  by Summer Y - Copyrignt 2005");
        header.MainVersion = FILEMAPSTREAMMAINVERSION;  //���汾��
        header.SubVersion = FILEMAPSTREAMSUBVERSION;    //�ΰ汾��
        header.Build = FILEMAPSTREAMBUILDVERSION;       //������
        dword written;
        if (!::WriteFile(m_hMapFile, (void*)&header,
            sizeof(FileStreamHeader), &written, NULL))
        {
            ::CloseHandle(m_hMapFile);
            this->WhenShitHappen();
            return;
        }
        //�������ĳ���Ϊ0
        dword streamsize = 0;
        ::WriteFile(m_hMapFile, (void*)&streamsize,
            sizeof(dword), &written, NULL);

        m_MapFileSize = ::GetFileSize(m_hMapFile, NULL);
    }
    else
    {
        //����ļ����ͺͰ汾�Ƿ����
        FileStreamHeader header;
        dword readed;
        ::ReadFile(m_hMapFile, (void*)&header,
            sizeof(FileStreamHeader), &readed, NULL);
        char desc[20];
        ::memcpy(desc, header.Description, 19);
        desc[19] = 0;
        if ((::strcmp(desc, FILEMAPSTREAMIDENTIFY))
            || (FILEMAPSTREAMMAINVERSION != header.MainVersion)
            || (FILEMAPSTREAMSUBVERSION != header.SubVersion)
            || (FILEMAPSTREAMBUILDVERSION != header.Build))
        {
            ::CloseHandle(m_hMapFile);
            this->WhenShitHappen();
            return;
        }
    }

    if (!this->MappingFile())
    {
        this->WhenShitHappen();
        return;
    }

    strcpy(m_MapFileName, pcFileName);

    this->SynchronizeModifyTime();
}

void CFileMapStream::Unload()
{
    if (m_pViewBase) ::UnmapViewOfFile(m_pViewBase);

    if (m_hMappingObject) ::CloseHandle(m_hMappingObject);

    if (m_hMapFile && (INVALID_HANDLE_VALUE != m_hMapFile))
        ::CloseHandle(m_hMapFile);

    this->WhenShitHappen();
}

void CFileMapStream::WhenShitHappen()
{
    m_pViewBase         = NULL;
    m_hMappingObject    = NULL;
    m_hMapFile          = INVALID_HANDLE_VALUE;
    m_MapFileName[0] = 0;
}

bool CFileMapStream::MappingFile()
{
    //����CreateFileMapping()�����ļ�ӳ�����
    dword MapObjProtect;
    if (m_ReadOnly)
    {
        MapObjProtect = PAGE_READONLY;
    }
    else
    {
        MapObjProtect = PAGE_READWRITE;
    }
    m_hMappingObject = ::CreateFileMapping(m_hMapFile,
        NULL, MapObjProtect, 0, 0, NULL);
    if (_invalid(m_hMappingObject))
    {
        ::CloseHandle(m_hMapFile);
        this->WhenShitHappen();
        return false;
    }

    //����MapViewOfFile()�㶨һ��View
    dword MapViewDesiredAccess;
    if (m_ReadOnly)
    {
        MapViewDesiredAccess = FILE_MAP_READ;
    }
    else
    {
        MapViewDesiredAccess = FILE_MAP_ALL_ACCESS;
    }
    m_pViewBase = (char*)::MapViewOfFile(m_hMappingObject,
        MapViewDesiredAccess, 0, 0, 0);
    if (_invalid(m_pViewBase))
    {
        ::CloseHandle(m_hMappingObject);
        ::CloseHandle(m_hMapFile);
        this->WhenShitHappen();
        return false;
    }

    //view��sizeof(FileStreamHeader)ƫ�ƴ���4bytes�������������Ч����
    m_StreamSize = *(dword*)(m_pViewBase + sizeof(FileStreamHeader));

    //����ʼλ����view��FILEMAPHEADERLENGTHƫ�ƴ�
    m_pStreamBase = m_pViewBase + FILEMAPHEADERLENGTH;

    //�ѹ���ָ��λ����Ϊ0
    m_pStreamPos = m_pStreamBase;

    return true;
}

bool CFileMapStream::IncreaseMappingSize(const dword delta)
{
    if (m_StreamSize + FILEMAPHEADERLENGTH + delta
        <= m_MapFileSize)
    {
        //����Ҫ����
        return true;
    }

    //ȡ����ǰ��mapping
    ::UnmapViewOfFile(m_pViewBase);
    ::CloseHandle(m_hMappingObject);

    //���湤��pos��ǰ��λ��
    dword posoffset = m_pStreamPos - m_pStreamBase;

    //���´�С����mapping
    dword filedelta = (dword(delta / m_MapFileIncreasSize) + 1)
        * m_MapFileIncreasSize;
    dword newsize = (filedelta + m_MapFileSize);

    //CreateFileMapping()
    dword MapObjProtect;
    if (m_ReadOnly)
    {
        MapObjProtect = PAGE_READONLY;
    }
    else
    {
        MapObjProtect = PAGE_READWRITE;
    }
    m_hMappingObject = ::CreateFileMapping(m_hMapFile, NULL,
        MapObjProtect, 0,  newsize, NULL);
    if (_invalid(m_hMappingObject))
    {
        //�ָ�mapping��ǰ�Ĵ�С
        if (!this->MappingFile()) throw;
        m_pStreamPos = m_pStreamBase + posoffset;
        return false;
    }
    m_MapFileSize = ::GetFileSize(m_hMapFile, NULL);

    //MapViewOfFile()
    dword MapViewDesiredAccess;
    if (m_ReadOnly)
    {
        MapViewDesiredAccess = FILE_MAP_READ;
    }
    else
    {
        MapViewDesiredAccess = FILE_MAP_ALL_ACCESS;
    }
    m_pViewBase = (char*)::MapViewOfFile(m_hMappingObject,
        MapViewDesiredAccess, 0, 0, 0);
    if (_invalid(m_pViewBase))
    {
        ::CloseHandle(m_hMappingObject);
        //�ָ�mapping��ǰ�Ĵ�С
        if (!this->MappingFile())
            throw;
        m_pStreamPos = m_pStreamBase + posoffset;
        return false;
    }

    //����ʼλ����view��FILEMAPHEADERLENGTHƫ�ƴ�
    m_pStreamBase = m_pViewBase + FILEMAPHEADERLENGTH;

    //�ָ�����pos��ԭ��λ��
    m_pStreamPos = m_pStreamBase + posoffset;

    return true;
}

void CFileMapStream::UpdateStreamLengthToFile()
{
    //Ϊ�����ļ�����޸�ʱ���ܹ������ı䣬����
    //ԭ����������һ������޷��ı��ļ����޸�ʱ��
    //*(dword*)(m_pViewBase + sizeof(FileStreamHeader))
    //    = this->m_StreamSize;

    dword written;
    ::SetFilePointer(m_hMapFile, sizeof(FileStreamHeader),
        NULL, FILE_BEGIN);
    ::WriteFile(m_hMapFile, (void*)(&m_StreamSize),
        sizeof(dword), &written, NULL);
}

bool __stdcall CFileMapStream::ReadOnly() const
{
    return m_ReadOnly;
}

dword __stdcall CFileMapStream::MovePos(const long delta,
    const StreamOffsetOrigin origin)
{
    dword posdelta;
    switch(origin)
    {
    case SOFROMBEGINNING:
        posdelta = (dword)delta;
        break;
    case SOFROMCURRENT:
        posdelta = (dword)(m_pStreamPos - m_pStreamBase + delta);
        break;
    case SOFROMEND:
        posdelta = m_StreamSize - (dword)delta;
        break;
    default: return 0;
    }
    if ((0 > posdelta) || (posdelta > m_StreamSize))
    {
        return 0;
    }

    m_pStreamPos = m_pStreamBase + posdelta;

    return m_pStreamPos - m_pStreamBase;
}

dword __stdcall CFileMapStream::GetPos() const
{
    return m_pStreamPos - m_pStreamBase;
}

dword CFileMapStream::Read(void* const p, const dword count)
{
//    if (_invalid(p)) return 0;
    dword realcount;
    dword remain = m_StreamSize - (m_pStreamPos - m_pStreamBase);
    if (remain >= count)
    {
        realcount = count;
    }
    else
    {
        realcount = remain;
    }
    memcpy(p, m_pStreamPos, realcount);
    m_pStreamPos += realcount;
    return realcount;
}

dword CFileMapStream::Write(const void* const p, const dword count)
{
    if (m_ReadOnly)
    {
        return 0;
    }

//    if (_invalid(p)) return 0;
    if (0 >= count)
    {
        return 0;
    }

    dword remain = m_StreamSize - (m_pStreamPos - m_pStreamBase);
    if (remain < count)
    {
        //buffer���Ȳ�������չ֮
        dword oldpos = m_pStreamPos - m_pStreamBase;
        if (!this->IncreaseMappingSize(oldpos + count - m_StreamSize))
        {
            //ӳ���С�޷���չ
            return 0;
        }
        m_StreamSize = oldpos + count;
        //m_pStreamPos = m_pStreamBase + oldpos;//IncreaseMappingSize()�Ѿ�����
    }
    memcpy(m_pStreamPos, p, count);
    m_pStreamPos += count;
    this->UpdateStreamLengthToFile();
    this->SynchronizeModifyTime();
    return count;
}

bool __stdcall CFileMapStream::SetSize(const dword size)
{
    if (m_ReadOnly)
    {
        return false;
    }

    if ((0 > size) || (size == m_StreamSize))
    {
        return false;
    }

    dword oldpos = m_pStreamPos - m_pStreamBase;
    long sizedelta = size - m_StreamSize;
    if (0 < sizedelta)
    {
        if (!this->IncreaseMappingSize(sizedelta))
        {
            //ӳ���С�޷���չ
            return false;
        }
    }

    if (size > m_StreamSize)
    {
        //char* p = m_pStreamBase + m_StreamSize;
        //memset(p, 0, (size - m_StreamSize));
        m_pStreamPos = m_pStreamBase + oldpos;
    }
    else
    {
        if (size > oldpos)
        {
            m_pStreamPos = m_pStreamBase + oldpos;
        }
        else
        {
            m_pStreamPos = m_pStreamBase + size;
        }
    }
    m_StreamSize = size;
    this->UpdateStreamLengthToFile();
    this->SynchronizeModifyTime();
    return true;
}

dword __stdcall CFileMapStream::GetSize() const
{
    return m_StreamSize;
}

bool __stdcall CFileMapStream::SaveToFile(const char* const filename)
{
    FILE* pFile = ::fopen(filename, "wb");
    if (_invalid(pFile)) return false;

    //��д��4bytes�ĳ�����Ϣ
    ::fwrite(&m_StreamSize, sizeof(dword), 1, pFile);
    dword numwritten = ::fwrite(m_pStreamBase,
        sizeof(char), m_StreamSize, pFile);
    ::fclose(pFile);
    if (numwritten != m_StreamSize) throw;

    return true;
}

bool __stdcall CFileMapStream::LoadFromFile(const char* const filename)
{
    if (m_ReadOnly) return false;

    this->SetSize(0);
    FILE* pFile = ::fopen(filename, "rb");
    if (_invalid(pFile)) return false;

    dword sizereaded;
    ::fread(&sizereaded, sizeof(dword), 1, pFile);//�ȶ���4bytes�ĳ�����Ϣ
    dword sizedelta = sizereaded - m_StreamSize;
    if (!this->IncreaseMappingSize(sizedelta))
    {
        //ӳ���С�޷���չ
        return false;
    }
    m_StreamSize = sizereaded;
    dword numreaded = ::fread(m_pStreamBase,
        sizeof(char), m_StreamSize, pFile);
    ::fclose(pFile);
    m_pStreamPos = m_pStreamBase;
    if (numreaded != m_StreamSize) throw;

    ::fclose(pFile);

    this->UpdateStreamLengthToFile();
    return true;
}

dword CFileMapStream::CopyDataTo(CStreamPtr const pDesCStream,
    const dword count)
{
    dword realcount;
    dword remain = m_StreamSize - (m_pStreamPos - m_pStreamBase);
    if (remain >= count)
    {
        realcount = count;
    }
    else
    {
        realcount = remain;
    }

    dword wrotebytes = pDesCStream->Write(m_pStreamPos, realcount);
    m_pStreamPos += wrotebytes;
    return wrotebytes;
}

const char* CFileMapStream::GetMapFileName() const
{
    return m_MapFileName;
}

bool CFileMapStream::SetIncreasSize(const dword size)
{
    if (m_ReadOnly)
    {
        return false;
    }

    if (size < FILEMAPMININCREASSIZE)
    {
        m_MapFileIncreasSize = FILEMAPMININCREASSIZE;
    }
    else if (size > FILEMAPMAXINCREASSIZE)
    {
        m_MapFileIncreasSize = FILEMAPMAXINCREASSIZE;
    }
    else
    {
        m_MapFileIncreasSize = size;
    }

    return true;
}

dword CFileMapStream::GetIncreasSize() const
{
    return m_MapFileIncreasSize;
}

bool CFileMapStream::ModifiedByOther() const
{
    BY_HANDLE_FILE_INFORMATION fileinfo;
    ::GetFileInformationByHandle(m_hMapFile, &fileinfo);
    if ((m_LastModifyTime.dwHighDateTime == fileinfo.ftLastWriteTime.dwHighDateTime)
        && (m_LastModifyTime.dwLowDateTime == fileinfo.ftLastWriteTime.dwLowDateTime))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CFileMapStream::SynchronizeModifyTime()
{
    //����ӳ���ļ�������޸�ʱ��
    BY_HANDLE_FILE_INFORMATION fileinfo;
    ::GetFileInformationByHandle(m_hMapFile, &fileinfo);
    m_LastModifyTime.dwHighDateTime = fileinfo.ftLastWriteTime.dwHighDateTime;
    m_LastModifyTime.dwLowDateTime = fileinfo.ftLastWriteTime.dwLowDateTime;
}

void CFileMapStream::Flush() const
{
}

bool CFileMapStream::ReMap()
{
    if ((!m_hMapFile) || (INVALID_HANDLE_VALUE == m_hMapFile)) {return false;}

    bool readonly = m_ReadOnly;
    char filename[3000];
    ::strcpy(filename, m_MapFileName);
    m_hMapFile              = INVALID_HANDLE_VALUE;
    m_hMappingObject        = NULL;
    m_pViewBase             = NULL;

    this->Unload();
    this->Initial(filename, readonly);

    return true;
}

const HANDLE CFileMapStream::GetMapFileHandle() const
{
    return m_hMapFile;
}

dword CFileMapStream::LoadFromInfo(CStreamPtr pStream)
{
    dword oldpos = pStream->GetPos();

    string filename;
    pStream->Read(filename);
    bool readonly = false;
    pStream->Read(&readonly, sizeof(bool));
    dword pos = 0;
    pStream->Read(&pos, sizeof(dword));
    dword offset = pStream->GetPos() - oldpos;

    if (filename == "")
    {
        this->WhenShitHappen();
        return offset;
    }

    this->Initial(filename.c_str(), readonly);
    this->MovePos(pos);

    return offset;
}

dword CFileMapStream::SaveInfo(CStreamPtr pStream) const
{
    dword oldpos = pStream->GetPos();

    string filename = m_MapFileName;
    pStream->Write(filename);
    pStream->Write(&m_ReadOnly, sizeof(bool));
    dword pos = this->GetPos();
    pStream->Write(&pos, sizeof(dword));

    return pStream->GetPos() - oldpos;
}

}
