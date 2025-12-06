#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
// assign workers to slots, n*d schedule
static bool assignShifts(
  size_t idx,
  const AvailabilityMatrix& avail,
  size_t dailyNeed,
  size_t maxShifts,
  std::vector<size_t>& shiftsUsed,
  DailySchedule& sched
);

// Add your implementation of schedule() and other helper functions here
static bool assignShifts(
  size_t idx,
  const AvailabilityMatrix& avail,
  size_t dailyNeed,
  size_t maxShifts,
  std::vector<size_t>& shiftsUsed,
  DailySchedule& sched
) {
  const size_t n = avail.size();
  const size_t k = avail[0].size();
  const size_t totalSlots = n * dailyNeed;

  // if all slots filled, true
  if (idx == totalSlots) {
    return true;
  }
  size_t day = idx / dailyNeed;

  for (size_t w = 0; w < k; w++) {
    // must be available
    if (!avail[day][w]) {
      continue;
    }
    // remaining shifts
    if (shiftsUsed[w] >= maxShifts) {
      continue;
    }
    // same worker not on same day
    if (std::find(sched[day].begin(), sched[day].end(), w) != sched[day].end()) {
      continue;
    }
    // assign worker to that slot
    sched[day].push_back(w);
    shiftsUsed[w]++;

    // go next slot
    if (assignShifts(idx + 1, avail, dailyNeed, maxShifts, shiftsUsed, sched)) {
      return true;
    }

    // backtrack
    shiftsUsed[w]--;
    sched[day].pop_back();
  }

  // nobody fits
  return false;
}
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    if (dailyNeed == 0) {
      sched.assign(numDays, std::vector<Worker_T>());
      return true;
    }

    if (numWorkers == 0) {
      return false;
    }

    if (numDays * dailyNeed > numWorkers * maxShifts) {
      return false;
    }

    sched.assign(numDays, std::vector<Worker_T>());
    std::vector<size_t> shiftsUsed(numWorkers, 0);

    return assignShifts(0, avail, dailyNeed, maxShifts, shiftsUsed, sched);
}

