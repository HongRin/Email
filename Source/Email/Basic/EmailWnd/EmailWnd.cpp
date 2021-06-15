#include "EmailWnd.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UEmailWnd::NativeConstruct()
{
	Super::NativeConstruct();

	EditableTextBox_Input->OnTextCommitted.AddDynamic(this, &UEmailWnd::OnEmailCommitted);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

}

bool UEmailWnd::GetIsAt(FString str)
{
	int32 atCount = 0;


	for (int i = 0; i < str.Len(); ++i)
	{
		if (str[i] == TCHAR(TEXT('@')))
			atCount++;
	}

	if (atCount != 0)
	{
		FString inQuotationMark = GetInQuotationMark(str);


		if (str.Compare(inQuotationMark) == 0) return true;

		int32 AtinQuotationMarkCount = 0;
		for (int i = 0; i < inQuotationMark.Len(); ++i)
		{
			if (inQuotationMark[i] == TCHAR(TEXT('@')))
				AtinQuotationMarkCount++;
		}

		if (atCount > AtinQuotationMarkCount) return true;
	}

	return false;
}

TTuple<FString, FString> UEmailWnd::GetIdAndDomainInAt(FString str)
{
	FString id;

	FString domain;

	// 마지막 @ 기준으로 문자열을 나눔

	FString read = TEXT("@");

	int32 index = str.Find(read, ESearchCase::IgnoreCase, ESearchDir::FromEnd, str.Len());

	for (int i = 0; i < index; ++i)
	{
		id.AppendChar(str[i]);
	}

	for (int i = (index + 1); i < str.Len(); ++i)
	{
		domain.AppendChar(str[i]);
	}

	return MakeTuple(id, domain);
}

void UEmailWnd::OuputErrorText(EErrorType errorType)
{
	switch (errorType)
	{
	case EErrorType::ET_ATZERO:
		Text_Output->SetText(FText::FromString(TEXT("@ 를 찾을 수 없습니다.")));
		break;
	case EErrorType::ET_SPECIALSYMBOL:
		Text_Output->SetText(FText::FromString(TEXT("인용 부호밖에 특수 문자가 있습니다.")));
		break;
	case EErrorType::ET_DMSPECIALSYMBOL:
		Text_Output->SetText(FText::FromString(TEXT("도메인에 특수 문자가 있습니다.")));
		break;
	case EErrorType::ET_IDAPERIOD:
		Text_Output->SetText(FText::FromString(TEXT("로컬 파트에 . 이 두개 이상 있습니다.")));
		break;
	case EErrorType::ET_DOMAINAPERIOD:
		Text_Output->SetText(FText::FromString(TEXT("도메인에 . 이 두개 이상 있습니다.")));
		break;
	case EErrorType::ET_IDBLANK:
		Text_Output->SetText(FText::FromString(TEXT("이메일 주소의 시작 혹은 끝 지점에 공백이 있습니다.")));
		break;
	case EErrorType::ET_IDLENGTH:
		Text_Output->SetText(FText::FromString(TEXT("로컬 파트가 너무 길거나 존재하지 않습니다.")));
		break;
	case EErrorType::ET_DOMAINLENGTH:
		Text_Output->SetText(FText::FromString(TEXT("도메인이 너무 길거나 존재 하지 않습니다.")));
		break;
	case EErrorType::ET_DOMAINHYPHEN:
		Text_Output->SetText(FText::FromString(TEXT("도메인의 앞과 끝에는 - 를 사용하실 수 없습니다.")));
		break;
	case EErrorType::ET_NONELABEL:
		Text_Output->SetText(FText::FromString(TEXT("도메인에 레이블이 존재하지 않습니다.")));
		break;
	}
}

EQuotationMarkType UEmailWnd::IsquotationMarkNumEven(FString str)
{
	int32 quotationMarkNum = 0;

	for (int32 i = 0; i < str.Len(); ++i)
	{
		if (str[i] == TCHAR(TEXT('\"')))
			++quotationMarkNum;
	}

	EQuotationMarkType type;

	if (quotationMarkNum == 0) type = EQuotationMarkType::QT_ZERO;
	else if (quotationMarkNum == 1) type = EQuotationMarkType::QT_ONE;
	else if ((quotationMarkNum % 2) == 0) type = EQuotationMarkType::QT_EVEN;
	else type = EQuotationMarkType::QT_ODD;

	return type;
}

FString UEmailWnd::GetInQuotationMark(FString str)
{
	if (IsquotationMarkNumEven(str) == EQuotationMarkType::QT_ONE) return FString(TEXT(""));
	if (IsquotationMarkNumEven(str) == EQuotationMarkType::QT_ZERO) return str;

	FString inQuotationMark;

	FString read = TEXT("\"");

	int32 startindex = str.Find(read, ESearchCase::IgnoreCase, ESearchDir::FromStart, 0);
	int32 endindex = str.Find(read, ESearchCase::IgnoreCase, ESearchDir::FromEnd, str.Len());

	for (int i = startindex; i < endindex + 1; ++i)
	{
		inQuotationMark.AppendChar(str[i]);
	}

	return inQuotationMark;
}

bool UEmailWnd::CheckIDSymbol(FString str)
{
	for (int i = 0; i < str.Len(); ++i)
	{
		int32 iStr = str[i];

		if (iStr < 33) return false;
		else if (iStr > 57 && iStr < 61) return false;
		else if (iStr == 62) return false;
		else if (iStr > 90 && iStr < 94) return false;
		else if (iStr > 126) return false;
	}
	
	return true;
}

bool UEmailWnd::CheckDomainSymbol(FString str)
{
	for (int i = 0; i < str.Len(); ++i)
	{
		int32 iStr = str[i];

		if (iStr < 45) return false;
		else if (iStr > 46 && iStr < 48) return false;
		else if (iStr > 57 && iStr < 65) return false;
		else if (iStr > 90 && iStr < 97) return false;
		else if (iStr > 122) return false;
	}

	return true;
}

bool UEmailWnd::CheckAperiod(FString str)
{
	int32 aperiodNum = 0;

	for (int32 i = 0; i < str.Len(); ++i)
	{
		if (str[i] == TCHAR(TEXT('.')))
			++aperiodNum;
	}

	return aperiodNum < 2;
}

bool UEmailWnd::CheckBlank(FString str)
{
	return (str[0] == TCHAR(TEXT(' ')) || str[str.Len() - 1] == TCHAR(TEXT(' ')));
}

bool UEmailWnd::CheckLength(FString str)
{
	return (!str.IsEmpty() && str.Len() <= 63);
}

bool UEmailWnd::CheckHyphen(FString str)
{
	return (str[0] == TCHAR(TEXT('-')) || str[str.Len() - 1] == TCHAR(TEXT('-')));
}

bool UEmailWnd::IsLabel(FString str)
{
	return (str[0] == TCHAR(TEXT('.')));
}

bool UEmailWnd::IDCheck(FString checkID)
{
	if (CheckLength(checkID))
	{
		FString inQuotationMark = GetInQuotationMark(checkID);

		if (checkID.Compare(inQuotationMark) == 0)
		{
			if (CheckIDSymbol(checkID))
			{
				if (CheckAperiod(checkID)) return true;
				else
				{
					OuputErrorText(EErrorType::ET_IDAPERIOD);
					return false;
				}
			}
			else
			{
				OuputErrorText(EErrorType::ET_SPECIALSYMBOL);
				return false;
			}
		}
		else
		{
			OuputErrorText(EErrorType::ET_SPECIALSYMBOL);
			return false;
		}
	}
	else
	{
		OuputErrorText(EErrorType::ET_IDLENGTH);
		return false;
	}

	return true;
}

bool UEmailWnd::DomainCheck(FString checkDomain)
{
	if (CheckLength(checkDomain))
	{
		if (CheckDomainSymbol(checkDomain))
		{
			if (CheckAperiod(checkDomain))
			{
				if (!CheckHyphen(checkDomain))
				{
					if (!IsLabel(checkDomain)) return true;
					else
					{
						OuputErrorText(EErrorType::ET_NONELABEL);
						return false;
					}
				}
				else
				{
					OuputErrorText(EErrorType::ET_DOMAINHYPHEN);
					return false;
				}
			}
			else
			{
				OuputErrorText(EErrorType::ET_DOMAINAPERIOD);
				return false;
			}
		}
		else
		{
			OuputErrorText(EErrorType::ET_DMSPECIALSYMBOL);
			return false;
		}
	}
	else
	{
		OuputErrorText(EErrorType::ET_DOMAINLENGTH);
		return false;
	}

	return true;
}

bool UEmailWnd::LabelCheck(FString checkLabel)
{
	

	return false;
}

void UEmailWnd::OnEmailCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터키를 눌렀다면
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// 입력한 email 을 FString 으로 담을 변수
		FString emailStr = Text.ToString();

		FString id;

		FString domain;

		if (GetIsAt(emailStr))
		{
			if (!CheckBlank(emailStr))
			{
				auto tupleData = GetIdAndDomainInAt(emailStr);
				id = tupleData.Get<0>();
				domain = tupleData.Get<1>();

				if (IDCheck(id) && DomainCheck(domain))
					Text_Output->SetText(FText::FromString(TEXT("ID[") + id + TEXT("]::Domain[") + domain + TEXT("]")));
			}
			else  OuputErrorText(EErrorType::ET_IDBLANK);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GetIsAt.false"));
			OuputErrorText(EErrorType::ET_ATZERO);
		}
	}
}