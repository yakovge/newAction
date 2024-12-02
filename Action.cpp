#include "Action.h"
#include "Simulation.h"
#include <iostream>
#include <stdexcept>
#include "Plan.h"
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Auxiliary.h"



using namespace std;
BaseAction:: BaseAction(): status(ActionStatus::COMPLETED), errorMsg("")){}


void BaseAction::complete(){
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error (string errorMsg){
    this->errorMsg = std::move(errorMsg);
    this->status = ActionStatus::ERROR;
}

ActionStatus BaseAction :: getStatus() const{
    return status;
}

const string& BaseAction::getErrorMsg () const{
    return errorMsg;
}


 AddFacility:: AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price,
  const int lifeQualityScore, const int economyScore, const int environmentScore)
  : facilityName(facilityName),facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore),
   economyScore(economyScore), environmentScore(environmentScore) {}

 AddFacility::AddFacility(const AddFacility& other)
    : facilityName(other.facilityName),
      facilityCategory(other.facilityCategory),
      price(other.price),
      lifeQualityScore(other.lifeQualityScore),
      economyScore(other.economyScore),
      environmentScore(other.environmentScore) {} 



AddFacility::AddFacility(AddFacility&& other) noexcept
    : facilityName(std::move(other.facilityName)),
      facilityCategory(other.facilityCategory),
      price(other.price),
      lifeQualityScore(other.lifeQualityScore),
      economyScore(other.economyScore),
      environmentScore(other.environmentScore) {
}


   void AddFacility:: act(Simulation &simulation) {
    try{
        simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
    complete();
    }
    catch (const std::exception& m){
        error("Facility already exists");
    }
   }

AddFacility* AddFacility:: clone() const{
    return new AddFacility(*this);
}

const string AddFacility::toString() const {
    // Convert FacilityCategory to string
    string categoryStr;
    switch (facilityCategory) {
        case FacilityCategory::LIFE_QUALITY:
            categoryStr = "Life Quality";
            break;
        case FacilityCategory::ECONOMY:
            categoryStr = "Economy";
            break;
        case FacilityCategory::ENVIRONMENT:
            categoryStr = "Environment";
            break;
        default:
            categoryStr = "Unknown";
            break;
    }
     return "AddFacility: [Facility Name: " + facilityName +
           ", Category: " + categoryStr  +
           ", Price: " + to_string(price) +
           ", Life Quality Impact: " + to_string(lifeQualityScore) +
           ", Economy Impact: " + to_string(economyScore) +
           ", Environment Impact: " + to_string(environmentScore) +
           "]";
}




SimulateStep:: SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep:: act(Simulation &simulation){
    for(int i = 0; i< numOfSteps; i++){
        simulation.step();
    }
}
const string SimulateStep:: toString() const{
    return "Number of steps: " + to_string(numOfSteps);    
        }
SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
    
}



 AddPlan:: AddPlan(const string &settlementName, const string &selectionPolicy)
 :settlementName(settlementName),selectionPolicy(selectionPolicy){}

void AddPlan:: act(Simulation &simulation){
    try{
        simulation.addPlan(settlementName,*selectionPolicy);
        complete();
    }
    catch(exception & e){
        error("Cannot create this plan");
    }
    
}
const string AddPlan::  toString() const{
    return "Settlement Name: " + settlementName + "\nSelection Policy: " 
    + selectionPolicy;
    
}
AddPlan* AddPlan::clone() const{
    return new AddPlan(*this);
}


AddSettlement::AddSettlement (const string &settlementName,SettlementType settlementType)
:settlementName(settlementName),settlementType(settlementType){}


void AddSettlement:: act(Simulation &simulation){
    if(simulation.isSettlementExists(settlementName)){
        error(settlementName + "is already exists");
        return;
    }
    Settlement* newSettlement = new Settlement(settlementName,settlementType);

    if(simulation.addSettlement(newSettlement)){
        complete();
    } else {
        delete newSettlement;
        error("Settlement already exists");
    }

AddSettlement* AddSettlement:: clone() const{
    return new AddSettlement(*this);
    
}
const string AddSettlement:: toString() const{
    return "settlement Name: ";
    

}


PrintPlanStatus::PrintPlanStatus(int planId)
:planId(planId){}

void PrintPlanStatus:: act(Simulation &simulation){
    try{
        Plan& plan = simulation.getPlan(planId);

        cout << "PlanID: " << planId << endl;
        cout << "SettlementName: " << plan.getSettlement().getName() << endl;
        cout << "PlanStatus: " << plan.getstatus() << endl;
        cout << "SelectionPolicy: " << plan.getSelectionPolicy() << endl;
        cout << "LifeQualityScore: " << plan.getEconomyScore() << endl;
        cout << "EconomyScore: " << plan.getEconomyScore() << endl;
        cout << "EnvironmentScore: " << plan.getEnvironmentScore()<<endl;

        vector<Facility*> facilities = facilityOptions = new  vector<Facility*>(plan.getFacilities());
        for(Facility* facility :facilities){
            cout <<"Facility Name: " <<  facility->getName() << endl;
            cout <<"Facility Status: " <<  StatusToString(facility.getStatus()) << endl;
        }
        complete();
    }
    catch (exception& m){
        error("Plan does not exist")
    }

}
        PrintPlanStatus *clone() const {
            return new PrintPlanStatus(*this);
        }

        const string toString() const{
            
        }
    