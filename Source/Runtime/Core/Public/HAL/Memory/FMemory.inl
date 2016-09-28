
void* FMemory::Malloc(SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		return MallocExternal(Count, Alignment);
	}
	//DoGamethreadHook(0);
	//FScopedMallocTimer Timer(0);
	return GMalloc->Malloc(Count, Alignment);
}

void* FMemory::Realloc(void* Original, SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		return ReallocExternal(Original, Count, Alignment);
	}
	//DoGamethreadHook(1);
	//FScopedMallocTimer Timer(1);
	return GMalloc->Realloc(Original, Count, Alignment);
}

void FMemory::Free(void* Original)
{
	if (!Original)
	{
		//FScopedMallocTimer Timer(3);
		return;
	}

	if (!GMalloc)
	{
		//FreeExternal(Original);
		return;
	}
	//DoGamethreadHook(2);
	//FScopedMallocTimer Timer(2);
	GMalloc->Free(Original);
}

SIZE_T FMemory::GetAllocSize(void* Original)
{
	if (!GMalloc)
	{
		return GetAllocSizeExternal(Original);
	}

	SIZE_T Size = 0;
	return GMalloc->GetAllocationSize(Original, Size) ? Size : 0;
}

SIZE_T FMemory::QuantizeSize(SIZE_T Count, uint32 Alignment)
{
	if (!GMalloc)
	{
		return Count;
	}
	return GMalloc->QuantizeSize(Count, Alignment);
}