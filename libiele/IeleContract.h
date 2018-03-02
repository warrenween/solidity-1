#pragma once

#include "IeleFunction.h"
#include "IeleGlobalVariable.h"
#include "IeleValue.h"
#include "SymbolTableListTraits.h"

#include "llvm/ADT/iterator_range.h"
#include "llvm/ADT/Twine.h"

namespace dev {
namespace iele {

class IeleValueSymbolTable;

class IeleContract :
  public IeleValue,
  public llvm::ilist_node_with_parent<IeleContract, IeleContract> {
public:
  // The type for the list of functions.
  using IeleFunctionListType = SymbolTableList<IeleFunction>;
  // The type for the list of global variables.
  using IeleGlobalVariableListType = SymbolTableList<IeleGlobalVariable>;
  // The type for the list of external contracts.
  using IeleContractListType = SymbolTableList<IeleContract>;

  // The function iterator.
  using iterator = IeleFunctionListType::iterator;
  // The function constant iterator.
  using const_iterator = IeleFunctionListType::const_iterator;

  // The global variable iterator.
  using global_iterator = IeleGlobalVariableListType::iterator;
  // The global variable constant iterator.
  using const_global_iterator = IeleGlobalVariableListType::const_iterator;

  // The external contract iterator.
  using contract_iterator = IeleContractListType::iterator;
  // The external contract constant iterator.
  using const_contract_iterator = IeleContractListType::const_iterator;

private:
  IeleFunctionListType IeleFunctionList;
  IeleGlobalVariableListType IeleGlobalVariableList;
  IeleContractListType IeleContractList;
  std::unique_ptr<IeleValueSymbolTable> SymTab;
  IeleContract *Parent;

  // Used by SymbolTableListTraits.
  void setParent(IeleContract *parent) { Parent = parent; }

  friend class SymbolTableListTraits<IeleContract>;

  // IeleContract ctor - If the (optional) IeleContract argument is specified,
  // the contract is automatically inserted into the end of the external
  // contract list for the given contract.
  //
  IeleContract(IeleContext *Ctx, const llvm::Twine &Name= "",
               IeleContract *C = nullptr);

public:
  IeleContract(const IeleContract&) = delete;
  void operator=(const IeleContract&) = delete;

  ~IeleContract();

  static IeleContract *Create(IeleContext *Ctx, const llvm::Twine &Name = "",
                              IeleContract *C = nullptr) {
    return new IeleContract(Ctx, Name, C);
  }

  inline const IeleContract *getParent() const { return Parent; }
  inline       IeleContract *getParent()       { return Parent; }

  // Get the underlying elements of the IeleContract.
  //
  const IeleFunctionListType &getIeleFunctionList() const {
    return IeleFunctionList;
  }
  IeleFunctionListType &getIeleFunctionList() {
    return IeleFunctionList;
  }

  static IeleFunctionListType
  IeleContract::*getSublistAccess(IeleFunction*) {
    return &IeleContract::IeleFunctionList;
  }

  const IeleGlobalVariableListType &getIeleGlobalVariableList() const {
    return IeleGlobalVariableList;
  }
  IeleGlobalVariableListType &getIeleGlobalVariableList() {
    return IeleGlobalVariableList;
  }

  static IeleGlobalVariableListType
  IeleContract::*getSublistAccess(IeleGlobalVariable*) {
    return &IeleContract::IeleGlobalVariableList;
  }

  const IeleContractListType &getIeleContractList() const {
    return IeleContractList;
  }
  IeleContractListType &getIeleContractList() {
    return IeleContractList;
  }

  static IeleContractListType
  IeleContract::*getSublistAccess(IeleContract*) {
    return &IeleContract::IeleContractList;
  }

  // Return the symbol table if any, otherwise nullptr.
  //
  inline IeleValueSymbolTable *getIeleValueSymbolTable() {
    return SymTab.get();
  }
  inline const IeleValueSymbolTable *getIeleValueSymbolTable() const {
    return SymTab.get();
  }

  // IeleFunction iterator forwarding functions.
  //
  iterator       begin()       { return IeleFunctionList.begin(); }
  const_iterator begin() const { return IeleFunctionList.begin(); }
  iterator       end  ()       { return IeleFunctionList.end();   }
  const_iterator end  () const { return IeleFunctionList.end();   }

  llvm::iterator_range<iterator> functions() {
    return make_range(begin(), end());
  }
  llvm::iterator_range<const_iterator> functions() const {
    return make_range(begin(), end());
  }

  size_t  size() const { return IeleFunctionList.size();  }
  bool   empty() const { return IeleFunctionList.empty(); }

  // IeleGlobalVariable iterator forwarding functions.
  //
  global_iterator global_begin() {
    return IeleGlobalVariableList.begin();
  }
  const_global_iterator global_begin() const {
    return IeleGlobalVariableList.begin();
  }
  global_iterator global_end() {
    return IeleGlobalVariableList.end();
  }
  const_global_iterator global_end() const {
    return IeleGlobalVariableList.end();
  }

  llvm::iterator_range<global_iterator> globals() {
    return make_range(global_begin(), global_end());
  }
  llvm::iterator_range<const_global_iterator> globals() const {
    return make_range(global_begin(), global_end());
  }

  size_t  global_size() const { return IeleGlobalVariableList.size();  }
  bool   global_empty() const { return IeleGlobalVariableList.empty(); }

  // IeleContract iterator forwarding functions.
  //
  contract_iterator contract_begin() {
    return IeleContractList.begin();
  }
  const_contract_iterator contract_begin() const {
    return IeleContractList.begin();
  }
  contract_iterator contract_end() {
    return IeleContractList.end();
  }
  const_contract_iterator contract_end() const {
    return IeleContractList.end();
  }

  llvm::iterator_range<contract_iterator> contracts() {
    return make_range(contract_begin(), contract_end());
  }
  llvm::iterator_range<const_contract_iterator> contracts() const {
    return make_range(contract_begin(), contract_end());
  }

  size_t  contract_size() const { return IeleContractList.size();  }
  bool   contract_empty() const { return IeleContractList.empty(); }

  void print(llvm::raw_ostream &OS, unsigned indent = 0) const override;

  // Method for support type inquiry through isa, cast, and dyn_cast.
  static bool classof(const IeleValue *V) {
    return V->getIeleValueID() == IeleValue::IeleContractVal;
  }
};

} // end namespace iele
} // end namespace dev