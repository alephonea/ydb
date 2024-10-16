#include "read_metadata.h"

namespace NKikimr::NOlap::NReader {

TDataStorageAccessor::TDataStorageAccessor(const std::unique_ptr<TInsertTable>& insertTable,
                                const std::unique_ptr<IColumnEngine>& index)
    : InsertTable(insertTable)
    , Index(index)
{}

std::shared_ptr<TSelectInfo> TDataStorageAccessor::Select(const TReadDescription& readDescription) const {
    if (readDescription.ReadNothing) {
        return std::make_shared<TSelectInfo>();
    }
    return Index->Select(readDescription.PathId,
                            readDescription.GetSnapshot(),
                            readDescription.PKRangesFilter);
}

std::vector<TCommittedBlob> TDataStorageAccessor::GetCommitedBlobs(const TReadDescription& readDescription, const std::shared_ptr<arrow::Schema>& pkSchema) const {
    return std::move(InsertTable->Read(readDescription.PathId, readDescription.GetSnapshot(), pkSchema));
}

}
