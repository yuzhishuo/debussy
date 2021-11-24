# 确定性跳表

Skip list最初由William Pugh提出，它是靠类似抛硬币的方式决定每个元素的层数，因此结果是随机的，它有概率(很小，但存在)出现最坏情况使得查找的时间复杂度退化为O(n)，最好的情况下是O(lgn)。随机的方式虽然有这种情况出现，但是它简单，不用人为的手段来平衡。因此，为了避免最坏的情况出现，有人在原来的基础上进行了改进，利用人为的手段来使得任何情况下整个跳跃表都处于相对平衡的状态。




## 跳表


```c++
int randomLevel() {
  int lv = 1;
  // MAXL = 32, S = 0xFFFF, PS = S * P, P = 1 / 4
  while ((rand() & S) < PS) ++lv;
  return min(MAXL, lv);
}
```